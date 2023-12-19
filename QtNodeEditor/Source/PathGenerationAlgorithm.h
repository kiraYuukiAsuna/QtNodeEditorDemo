#pragma once

#include <iostream>
#include <vector>
#include <cmath>

std::vector<double> add(const std::vector<double> &vectorA, const std::vector<double> &vectorB) {
    return {vectorA[0] + vectorB[0], vectorA[1] + vectorB[1]};
}

std::vector<double> multiply(const std::vector<double> &vector, double k) {
    return {vector[0] * k, vector[1] * k};
}

std::vector<double> vectorFromPoints(const std::vector<double> &pointA, const std::vector<double> &pointB) {
    return {pointB[0] - pointA[0], pointB[1] - pointA[1]};
}

bool isParallel(const std::vector<double> &vectorA, const std::vector<double> &vectorB) {
    return (vectorA[0] * vectorB[1] - vectorA[1] * vectorB[0]) == 0;
}

double dot(const std::vector<double> &vectorA, const std::vector<double> &vectorB) {
    return vectorA[0] * vectorB[0] + vectorA[1] * vectorB[1];
}

std::vector<double> getUnitVector(const std::vector<double> &vector) {
    double m = sqrt(vector[0] * vector[0] + vector[1] * vector[1]);
    return {vector[0] / m, vector[1] / m};
}

std::vector<std::vector<double> > generateConnectionPoints(const std::vector<double> &entryPoint,
                                                           const std::vector<double> &entryDirection,
                                                           double entryExt, const std::vector<double> &exitPoint,
                                                           const std::vector<double> &exitDirection, double exitExt,
                                                           double turnRatio) {
    std::vector<double> exitDirectionCopy = exitDirection;

    if (exitDirectionCopy[0] == 0 && exitDirectionCopy[1] == 0) {
        std::vector<double> entryToExit = vectorFromPoints(exitPoint, entryPoint);
        if (std::abs(entryToExit[0]) > std::abs(entryToExit[1])) {
            exitDirectionCopy = {entryToExit[0] / std::abs(entryToExit[0]), 0};
        } else {
            exitDirectionCopy = {0, entryToExit[1] / std::abs(entryToExit[1])};
        }
    }

    std::vector<double> pathStartP = add(entryPoint, multiply(entryDirection, entryExt));
    std::vector<double> pathEndP = add(exitPoint, multiply(exitDirectionCopy, exitExt));

    exitDirectionCopy = multiply(exitDirectionCopy, -1);

    std::vector<double> pathHorizenVec = {pathEndP[0] - pathStartP[0], 0};
    std::vector<double> pathVerticalVec = {0, pathEndP[1] - pathStartP[1]};

    std::vector<std::vector<double> > comp = {pathHorizenVec, pathVerticalVec};

    std::vector<double> pathStart;
    std::vector<double> startParallelVec;
    for (const auto &vec: comp) {
        if (isParallel(vec, entryDirection)) {
            startParallelVec = vec;
            break;
        }
    }

    if (dot(startParallelVec, entryDirection) > 0) {
        pathStart = startParallelVec;
    } else {
        pathStart = comp[0] == startParallelVec ? comp[1] : comp[0];
    }

    std::vector<double> pathEnd;
    std::vector<double> endParallelVec;
    for (const auto &vec: comp) {
        if (isParallel(vec, exitDirectionCopy)) {
            endParallelVec = vec;
            break;
        }
    }

    if (dot(endParallelVec, exitDirectionCopy) > 0) {
        pathEnd = endParallelVec;
    } else {
        pathEnd = comp[0] == endParallelVec ? comp[1] : comp[0];
    }

    int splitNum = dot(pathStart, pathEnd) > 0 ? 2 : 1;

    std::vector<double> pathMiddle = comp[0] == pathEnd ? comp[1] : comp[0];

    std::vector<std::vector<double> > points;

    points.push_back(entryPoint);
    points.push_back(pathStartP);

    if (splitNum == 1) {
        std::vector<double> point1 = add(pathStartP, pathStart);
        std::vector<double> dir1 = getUnitVector(pathStart);
        std::vector<double> point2 = add(point1, pathEnd);
        std::vector<double> dir2 = getUnitVector(pathEnd);

        points.push_back(point1);
        points.push_back(point2);
    } else {
        std::vector<double> point1 = add(pathStartP, multiply(pathStart, turnRatio));
        std::vector<double> dir1 = getUnitVector(pathStart);
        std::vector<double> point2 = add(point1, pathMiddle);
        std::vector<double> dir2 = getUnitVector(pathMiddle);
        std::vector<double> point3 = add(point2, multiply(pathEnd, 1 - turnRatio));
        std::vector<double> dir3 = getUnitVector(pathEnd);

        points.push_back(point1);
        points.push_back(point2);
        points.push_back(point3);
    }

    points.push_back(exitPoint);

    return points;
}

std::vector<double> getUnictVecByStraight(const std::vector<double> &vector) {
    if (vector[0] == 0 && vector[1] == 0) {
        // Handle the case where the vector is a zero vector.
        return {0, 0}; // You may need to decide the appropriate handling here.
    } else if (vector[0] == 0) {
        return {0, vector[1] / std::abs(vector[1])};
    } else if (vector[1] == 0) {
        return {vector[0] / std::abs(vector[0]), 0};
    } else {
        return getUnitVector(vector);
    }
}

// int main() {
//     std::vector<std::vector<double> > result =
//             generateConnectionPoints({0, 0}, {1, 0}, 10, {200, 100}, {1, 0}, 10, 0.5);
//
//     std::cout << "Generated Points:\n";
//     for (const auto &point: result) {
//         std::cout << "(" << point[0] << ", " << point[1] << ")\n";
//     }
//
//     return 0;
// }
