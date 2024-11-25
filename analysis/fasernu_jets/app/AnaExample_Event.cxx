#include "FaserNu_Jets/GenieEvent.h"
#include "FaserNu_Jets/GenieAna.h"
#include "fastjet/ClusterSequence.hh"

#include <iostream>
#include <vector>

int main()
{
	std::cout << "Wellcome to GenieEvent" << std::endl;

    std::string input = "/storage/hepgrp/ekajomov/FASER_nu/jet_reconstruction/FASERnu.FK.2010GENIE.200000.gfaser.root";
    std::string output = "plots.pdf";
    GenieAna a;
    a.setInput(input);
    a.setOutput(output);
	a.init();
    a.process();
    a.close();

}
