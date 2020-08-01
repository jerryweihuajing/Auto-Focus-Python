// -*- coding: utf-8 -*-
/******************************************************************************
@author: Wei Huajing
@company: KAMERAWERK
@e-mail: jerryweihuajing@126.com

@title: Header-Operation on Import
******************************************************************************/

#include "..\init.h"

#ifndef _OPERATION_IMPORT_H_
#define _OPERATION_IMPORT_H_

//Calculate the path of all the files under the path
vector<string> VectorFilesPath(string& folder_path);

//Get bgr image matrix and construct a vector
vector<Mat> VectorImgBGR(string& folder_path);

//Get gray image matrix and construct a vector
vector<Mat> VectorImgGray(string& folder_path);

#endif