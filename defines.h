#ifndef DEFINES_H
#define DEFINES_H

/////////////////////////////////////////////////////////////////////////
//
//		File:   Defines.h
//
//		Author: Mat Buckland
//
//		Desc:   #defines for the code project 'pathfinder'
//
/////////////////////////////////////////////////////////////////////////

#define WINDOW_WIDTH	500
#define WINDOW_HEIGHT	400

//lhx
#define MAP_BORDER		20


//lihx
#define MAX_BARRIERS	6
#define BARRIER_COUNT1	4
#define BARRIER_COUNT2	3
#define BARRIER_COUNT3	4
#define BARRIER_COUNT4	3
#define BARRIER_COUNT5	4
#define BARRIER_COUNT6	4
//lhx test point
#define START_X			80
#define START_Y			57
#define END_X			368
#define END_Y			249

#define CROSSOVER_RATE	0.7f
#define MUTATION_RATE	0.001f
#define POP_SIZE		20
#define CHROMO_LENGTH	20
#define GENERATIONS		30

#define UM_SPAWN_NEW (WM_USER + 0)
#endif