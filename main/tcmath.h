//
//  tcmath.h
//  MazeRunner
//
//  Created by Akiva Lipshitz on 5/13/15.
//  Copyright (c) 2015 Akiva Lipshitz. All rights reserved.
//

#ifndef __MazeRunner__tcmath__
#define __MazeRunner__tcmath__

#ifdef __APPLE__
#include <stdio.h>
//Because stupid math.h had to give min() and max()
//wierd names.
#define min(n,b) (fmin(n,b))
#define max(n,b) (fmax(n,b))
#endif

typedef int Bool;
#define true 1
#define false 0
/*-------( Math functions )-------*/
int cut(int n, int mag, int _min, int _max);

int absv(int n);


#endif /* defined(__MazeRunner__tcmath__) */

