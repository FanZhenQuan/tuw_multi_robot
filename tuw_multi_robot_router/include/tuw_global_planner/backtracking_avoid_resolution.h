/*
 * Copyright (c) 2017, <copyright holder> <email>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY <copyright holder> <email> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> <email> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#ifndef BACKTRACKING_AVOID_RESOLUTION_H
#define BACKTRACKING_AVOID_RESOLUTION_H

#include <tuw_global_planner/segment.h>
#include <tuw_global_planner/path_coordinator.h>
#include <tuw_global_planner/collision_resolution.h>

class BacktrackingAvoidResolution : public CollisionResolution
{
public:     BacktrackingAvoidResolution(std::shared_ptr<Path_Coordinator> _path_querry, std::shared_ptr<PotentialCalculator> _pCalc, int _timeoverlap) : CollisionResolution(_path_querry, _pCalc,_timeoverlap)  { }
public:     std::vector<std::shared_ptr<Segment>> resolve(std::shared_ptr< Segment > _current, std::shared_ptr< Segment > _next, std::shared_ptr< Segment > _end, int _collision, int _robot_radius);
public:		void reset();
private:	void avoid(std::shared_ptr< Segment > _current, std::shared_ptr< Segment > _next, std::shared_ptr< Segment > _end, float _newPot, int _robot_radius, std::vector<std::shared_ptr<Segment>> &retVal);
private:	void trackBack(std::shared_ptr< Segment > _current, std::shared_ptr< Segment > _next, std::shared_ptr< Segment > _end, int _collision, int _robot_radius, float _newPot, std::vector<std::shared_ptr<Segment>> &retVal);
private:	void moveSegment(std::shared_ptr<Segment> newCurrent, std::shared_ptr<Segment> cross_next, int _robot_radius, int _coll, std::vector<std::shared_ptr<Segment>> &retVal);
private:	void avoidStart(std::shared_ptr< Segment > _current, std::shared_ptr< Segment > _next, float _newPot, int _robot_radius, std::vector<std::shared_ptr<Segment>> &retVal);
private:	void avoidEnd(std::shared_ptr< Segment > _current, std::shared_ptr< Segment > _next, float _newPot, int _robot_radius, std::vector<std::shared_ptr<Segment>> &retVal, int _collision);
public:     void clear();

private:	std::vector<int> avoidedSegments_;
private:    std::vector<std::shared_ptr<Segment>> createdSegmements_;
};

#endif // HEURISTIC_H
