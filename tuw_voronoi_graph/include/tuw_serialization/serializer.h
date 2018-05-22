
#ifndef _TUW_VORONOI_SERIALIZER
#define _TUW_VORONOI_SERIALIZER

#include <nav_msgs/OccupancyGrid.h>
#include <memory>
#include <opencv/cv.h>
#include <tuw_voronoi_graph/segment.h>

#include <fstream>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#define DEFAULT_MAP_NAME    "voronoi_map"
#include <eigen3/Eigen/Dense>
#include <opencv/cv.hpp>
#include <boost/filesystem.hpp>
#include <boost/functional/hash.hpp>

namespace tuw_graph
{

    class PointSerializer
    {
        public:
            PointSerializer() : PointSerializer(0, 0)
            {}
            PointSerializer(float _x, float _y)
            {
                x = _x;
                y = _y;
            }
            PointSerializer(Eigen::Vector2d p)
            {
                x = p[0];
                y = p[1];
            }
            float x;
            float y;
        private:
            friend class boost::serialization::access;
            template<class archive> void serialize(archive & ar, const unsigned int version)
            {
                using boost::serialization::make_nvp;
                ar & boost::serialization::make_nvp("x", x);
                ar & boost::serialization::make_nvp("y", y);
            }
    };

    class GraphInfo
    {
        public:
            GraphInfo() : Origin(0, 0)
            {};
            GraphInfo(Eigen::Vector2d _pt, float _resolution, int _nrSegments_) : Origin(_pt)
            {
                Resolution = _resolution;
                SegmentLength = _nrSegments_;
            }
            PointSerializer Origin;
            float Resolution;
            int SegmentLength;

        private:
            friend class boost::serialization::access;
            template<class archive> void serialize(archive & ar, const unsigned int version)
            {
                using boost::serialization::make_nvp;
                ar & boost::serialization::make_nvp("Origin", Origin);
                ar & boost::serialization::make_nvp("Resolution", Resolution);
                ar & boost::serialization::make_nvp("SegmentLength", SegmentLength);
            }
    };

    class TreeInfo
    {
        public:
            TreeInfo(std::vector<Segment> _segs) : TreeInfo(_segs.size())
            {
                for(uint32_t i = 0; i < _segs.size(); i++)
                {
                    int * pred = Predecessors.get();
                    int * succ = Successors.get();
                    int * point = Points.get();
                    pred[i] = _segs[i].GetPredecessors().size();
                    succ[i] = _segs[i].GetSuccessors().size();
                    point[i] = _segs[i].GetPath().size();
                }
            }
            TreeInfo(int _length)
            {
                Predecessors.reset(new int[_length]);
                Successors.reset(new int[_length]);
                Points.reset(new int[_length]);
                Length = _length;
                //predLength = _length;
                
            }
            int Length;
            std::unique_ptr<int> Predecessors;
            std::unique_ptr<int> Successors;
            std::unique_ptr<int> Points;

        private:
            //int predLength;
            //int succLength;
            //int pointsLength;
            friend class boost::serialization::access;
            template<class archive> void serialize(archive & ar, const unsigned int version)
            {
                using boost::serialization::make_nvp;
                ar & boost::serialization::make_nvp("Length", Length);
                ar & boost::serialization::make_array<int>(Predecessors.get(), Length);
                ar & boost::serialization::make_array<int>(Successors.get(), Length);
                ar & boost::serialization::make_array<int>(Points.get(), Length);
            }
    };



    class SegmentSerializer
    {
        public:
            SegmentSerializer() : SegmentSerializer(0, 0, 0)
            {
            }

            SegmentSerializer(int _predLength, int _succLength, int _pointLength)
            {
                predecessorLength = _predLength;
                successorLength = _succLength;
                pointLength = _pointLength;
                predecessors.reset(new int[predecessorLength]);
                successors.reset(new int[successorLength]);
                points.reset(new PointSerializer[pointLength]);

            }

            SegmentSerializer(const Segment &_s) :
                SegmentSerializer(_s.GetId(), _s.GetPredecessors(), _s.GetSuccessors(), _s.GetMinPathSpace(), _s.GetPath())
            {
            }

            SegmentSerializer(const uint32_t _id, std::vector<int32_t> _predecessors, std::vector<int32_t> _successors, float _minDistance, std::vector<Eigen::Vector2d> _points):
                SegmentSerializer(_predecessors.size(), _successors.size(), _points.size())
            {
                id = _id;
                minDistance = _minDistance;

                int *pred = predecessors.get();
                int *succ = successors.get();
                PointSerializer *pts = points.get();


                for(int i = 0; i < predecessorLength; i++)
                {
                    pred[i] = _predecessors[i];
                }

                for(int i = 0; i < successorLength; i++)
                {
                    succ[i] = _successors[i];
                }

                for(uint32_t i = 0; i < _points.size(); i++)
                {
                    PointSerializer p(_points[i]);
                    pts[i] = p;
                }
            }

            int id;
            int predecessorLength;
            int successorLength;
            std::unique_ptr<int> predecessors;
            std::unique_ptr<int> successors;
            float minDistance;
            int pointLength;
            std::unique_ptr<PointSerializer> points;

        private:
            friend class boost::serialization::access;
            template<class archive> void serialize(archive & ar, const unsigned int version)
            {
                using boost::serialization::make_nvp;
                ar & boost::serialization::make_nvp("id", id);
                ar & boost::serialization::make_nvp("predecessorLength", predecessorLength);
                ar & boost::serialization::make_nvp("successorLength", successorLength);
                ar & boost::serialization::make_nvp("minDistance", minDistance);
                ar & boost::serialization::make_nvp("pointLength", pointLength);
                ar & boost::serialization::make_array<int>(predecessors.get(), predecessorLength);
                ar & boost::serialization::make_array<int>(successors.get(), successorLength);
                ar & boost::serialization::make_array<PointSerializer>(points.get(), pointLength);
            }
    };


    class GraphSerializer
    {
        public:
            GraphSerializer(std::vector<SegmentSerializer> &_segments)
            {
                segments_ = &_segments[0];
                Length = _segments.size();
            }
            int Length;
            SegmentSerializer *segments_;
        private:
            friend class boost::serialization::access;
            template<class archive> void serialize(archive & ar, const unsigned int version)
            {
                using boost::serialization::make_nvp;
                ar & boost::serialization::make_nvp("pointLength", Length);
                ar & boost::serialization::make_array<SegmentSerializer>(segments_, Length);
            }
    };

    class Serializer
    {
        public:
            Serializer();
            void save(const std::string &_mapPath, const std::vector<Segment> &_segs, const Eigen::Vector2d &_origin, const float &_resolution);
            bool load(const std::string &_mapPath, std::vector<Segment> &_segs, Eigen::Vector2d &_origin, float &_resolution);
            size_t getHash(const std::vector<signed char> &_map, Eigen::Vector2d _origin, float _resolution);
    };

}

#endif
