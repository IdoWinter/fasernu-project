#pragma once

#include <vector>
#include <iostream>
#include "FaserNu_Jets/CustomJetAlgorithm.h"
#include <utility>


std::pair<int, int> count_mesons_and_baryons(std::vector<Particle*> particles);