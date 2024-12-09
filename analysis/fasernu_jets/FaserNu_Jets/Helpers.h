#pragma once

#include <vector>
#include <iostream>
#include "FaserNu_Jets/CustomJetAlgorithm.h"
#include <TVectorD.h>
#include <utility>
#include <functional>


std::pair<int, int> count_mesons_and_baryons(std::vector<Particle*> particles);

double ComputeRij(Particle* p1, Particle* p2);

TVectorD linspace(double start, double end, int num);

TVectorD calcFunc(TVectorD x, std::function<double(double)> f);