//
//  tctypes.h
//  MazeRunner
//
//  Created by Akiva Lipshitz on 5/14/15.
//  Copyright (c) 2015 Akiva Lipshitz. All rights reserved.
//

#ifndef MazeRunner_tctypes_h
#define MazeRunner_tctypes_h

typedef struct Vec4 {
    int& operator[](int i) { return vec[i]; }
    int vec[4];
} Vec4;

typedef struct Vec4f {
    float& operator[](int i) { return vec[i]; }
    float vec[4];
} Vec4f;

#endif
