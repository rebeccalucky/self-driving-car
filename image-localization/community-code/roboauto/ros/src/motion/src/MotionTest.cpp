/*
 *  Copyright 2016 RoboAuto team, Artin
 *  All rights reserved.
 *
 *  This file is part of RoboAuto HorizonSlam.
 *
 *  RoboAuto HorizonSlam is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  RoboAuto HorizonSlam is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with RoboAuto HorizonSlam.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "MotionTest.h"

double Motion::MotionTest::predictSpeedByNN(cv::Mat flow){
    double ret = net_.computeOutput(Sample::createSample(flow))[0];
    return ret;
};

void Motion::MotionTest::loadNetFromFile(const std::string &file){
    std::cout << "Loading NN from the file: \"" << file << "\"\n";

    std::ifstream myfile(file, std::ifstream::in);
    if (myfile.is_open()) {
        std::string json((std::istreambuf_iterator<char>(myfile)),
                         std::istreambuf_iterator<char>());

        net_ = *NeuralNetwork::FeedForward::Network::deserialize(SimpleJSON::JSONParser::parseObject(json)).release();
        std::cout << "done...\n";
    } else std::cout << "Unable to open file";
};

void Motion::MotionTest::initNN(NeuralNetwork::FeedForward::Network& n,const std::string& file) {
    // we have a file
    if (!file.empty()) {
        loadNetFromFile(file);
    } else {
        NeuralNetwork::ActivationFunction::Sigmoid a(-1);
        n.appendLayer(100, a);
        n.appendLayer(1, a);
        n.randomizeWeights();
    }
};
