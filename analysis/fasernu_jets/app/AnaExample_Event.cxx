#include "FaserNu_Jets/GenieEvent.h"
#include "FaserNu_Jets/GenieAna.h"
#include "fastjet/ClusterSequence.hh"

#include <iostream>
#include <vector>

int main()
{
	std::cout << "Welcome to GenieEvent" << std::endl;

    std::string input = "/storage/hepgrp/idow/FASER_nu/fasernu-project/jet_reconstruction/FASERnu.FK.2010GENIE.200000.gfaser.root";
    std::string output = "plots.pdf";
    GenieAna a;
    a.setInput(input);
    a.setOutput(output);
	a.init();
    a.process();
    a.close();

}
