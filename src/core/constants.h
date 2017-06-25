//
// Created by oliver on 21.06.17.
//

#ifndef TBBTEST_CONSTANTS_H
#define TBBTEST_CONSTANTS_H

const int CELL_SIZE = 12;

const int SWARM_SIZE = 400;
const int ALLIGNMENT_RANGE =    10;
const int COHESION_RANGE =      7;
const int SEPARATION_RANGE =    1;
const int RETURN_HOME_RANGE =   55;

const int COHESION_CELLS =      COHESION_RANGE / CELL_SIZE   + 1;
const int ALLIGNMENT_CELLS =    ALLIGNMENT_RANGE / CELL_SIZE + 1;
const int SEPARATION_CELLS =    SEPARATION_RANGE / CELL_SIZE + 1;
const int RETURN_HOME_CELLS =    RETURN_HOME_RANGE / CELL_SIZE +1;


const float min_speed = 5;
const float max_speed = 12;


#endif //TBBTEST_CONSTANTS_H
