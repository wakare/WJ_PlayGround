//
// Created by jackjwang on 2021/6/19.
//

#include <iostream>
#include <string>
#include "CMakeConfig.h"
#include "Eigen/Dense"

int main()
{
    char CMakeInfo[200];
    memset(CMakeInfo, 0, sizeof CMakeInfo);

    sprintf(CMakeInfo, "WJ_PlayGround_VERSION_MAJOR %d %d", WJ_PlayGround_VERSION_MAJOR, WJ_PlayGround_VERSION_MINOR);
    std::cout << CMakeInfo << std::endl;

    Eigen::Vector4f TestVector0 = {0.0f, 0.0f, 1.0f, 1.0f};
    Eigen::Vector4f TestVector1 = {0.0f, 0.0f, 1.0f, 1.0f};

    float result = TestVector0.dot(TestVector1);
    std::cout << "EigenTest dot: " << result << std::endl;

    return 0;
}