#pragma once
#include <string>
#include "CoarseDetection.h"
#include "FineDetection.h"

using namespace std;
extern string img_format;


int getProposal(string filename);
#define GET_DATA

