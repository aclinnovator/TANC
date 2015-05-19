//
//  tcmath.c
//  MazeRunner
//
//  Created by Akiva Lipshitz on 5/13/15.
//  Copyright (c) 2015 Akiva Lipshitz. All rights reserved.
//

#include "tcmath.h"

int cut(int n, int mag, int _min, int _max){
    int res;
    
    if       (n+mag < _min){ res =  _min; }
    else if  (n+mag > _max){ res = _max;  }
    else                   { res =  n  + mag; }
    
    return res;
}

int absv(int n){
    return (n < 0 ? -n : n);
}