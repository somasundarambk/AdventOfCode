#pragma once

#include<list>
#include<stack>
#include<queue>
#include <string>
#include <set>
#include <array>
#include <algorithm>
#include <unordered_map>
#include <tuple>
#include <iostream>
#include <fstream>
#include <map>
#include <functional>

using namespace std;

class ADV23
{
public:
    ADV23()
    {
        PopulateInput();
        GetMostSpheresWithinSphere();
    }

    void GetMostSpheresWithinSphere()
    {
        // Step 1 => I deduced that prunedItems are those that overlap with most other bot's range
        std::set<tuple<long long, long long, long long, long long>> prunedItems;
        for (auto& item1 : bots)
        {
            std::set<tuple<long long, long long, long long, long long>> curSet;
            curSet.insert(item1);
            for (auto& item2 : bots)
            {
                if (item1 != item2)
                {
                    if (DoSpheresOverlap(item1, item2))
                    {
                        curSet.insert(item2);
                    }
                }
            }
            if (curSet.size() > 984) // By calibration
            {
                prunedItems.insert(item1);
            }
        }

        // Step 2 => I went through each of them and validated that they all overlap with each other
        cout << "SIZE OF MASTER = " << prunedItems.size(); // This was 985
        for (auto item1 : prunedItems)
        {
            for (auto item2 : prunedItems)
            {
                if (!DoSpheresOverlap(item1, item2))
                {
                    cout << "Oh Bugger!";
                }
            }
        }

        // Step 3 => I validated that none of the reminders can indeed be added
        set<tuple<long long, long long, long long, long long>> leftOvers;
        for (auto bot : bots)
        {
            if (prunedItems.find(bot) == prunedItems.end())
            {
                leftOvers.insert(bot);
            }
        }
        cout << "\nLeft over count = " << leftOvers.size(); // 15
        set<tuple<long long, long long, long long, long long>> addableLeftOver;
        for (auto leftOver : leftOvers)
        {
            bool canAdd = true;
            for (auto bot : bots)
            {
                if (!DoSpheresOverlap(bot, leftOver))
                {
                    canAdd = false;
                    break;
                }
            }
            if (canAdd)
                addableLeftOver.insert(leftOver);
        }
        cout << "\n Addable left over over count = " << addableLeftOver.size(); // Size 0

        // Step 4 => Found the minimum point of intersection of all these bots by looking at their least x, y and z values
        auto minX = numeric_limits<long long>::max();
        auto minY = numeric_limits<long long>::max();
        auto minZ = numeric_limits<long long>::max();
        auto maxX = numeric_limits<long long>::min();
        auto maxY = numeric_limits<long long>::min();
        auto maxZ = numeric_limits<long long>::min();

        for (auto bot : prunedItems)
        {
            minX = min(minX, get<0>(bot) + get<3>(bot));
            minY = min(minX, get<1>(bot) + get<3>(bot));
            minZ = min(minX, get<2>(bot) + get<3>(bot));
            maxX = max(maxX, get<0>(bot) - get<3>(bot));
            maxY = max(maxX, get<1>(bot) - get<3>(bot));
            maxZ = max(maxX, get<2>(bot) - get<3>(bot));
        }

        cout << "\nMin = (" << minX << "," << minY << "," << minZ << ") \n"; // Min = (56332497,56332497,56332497)
        cout << "\nMax = (" << maxX << "," << maxY << "," << maxZ << ") \n"; // Max = (51105926,51105926,51105926)

        cout << "\n Answer = " << GetManhattanDistance(make_tuple(maxX, maxY, maxZ, 0), make_tuple(0, 0, 0, 0)); // 153317778 !!!Wrong :-( !!!
    }

    long long GetManhattanDistance(tuple<long long, long long, long long, long long> source, tuple<long long, long long, long long, long long> destination)
    {
        long long x1, x2, y1, y2, z1, z2, r1, r2;
        tie(x1, y1, z1, r1) = source;
        tie(x2, y2, z2, r2) = destination;
        return abs(x1 - x2) + abs(y1 - y2) + abs(z1 - z2);
    }

    bool DoSpheresOverlap(tuple<long long, long long, long long, long long> source, tuple<long long, long long, long long, long long> destination)
    {
        long long x1, x2, x3, y1, y2, y3, z1, z2, z3, r1, r2;
        tie(x1, y1, z1, r1) = source;
        tie(x2, y2, z2, r2) = destination;
        return (get<3>(source) + get<3>(destination) >= GetManhattanDistance(source, destination));
    }

    void PopulateInput(string fileName = "23_2.txt")
    {

        ifstream file(fileName);
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                bots.insert(GetBot(line));
            }
        }
    }

    tuple<long long, long long, long long, long long> GetBot(string line)
    {
        long long a, b, c, r;
        std::string::size_type sz;
        auto sp2 = -1;
        auto sp1 = line.find("<", sp2 + 1) + 1;
        sp2 = line.find(",", sp1);
        a = stoi(line.substr(sp1, sp2 - sp1), &sz);
    
        sp1 = line.find(",", sp2 + 1) + 1;
        b = stoi(line.substr(sp2 + 1, sp1 - sp2 - 2), &sz);
    
        sp2 = line.find(">", sp1 + 1);
        c = stoi(line.substr(sp1, sp2 - sp1), &sz);
    
        sp1 = line.find("r=") + 2;
        r = stoi(line.substr(sp1, line.size() - sp1), &sz);
        return make_tuple(a, b, c, r);
    }

    std::set<tuple<long long, long long, long long, long long>> bots;
    list<tuple<long long, long long, long long, long long>> points;
};



//std::set<tuple<long long, long long, long long, long long>> DeduceByReduction()
//{
//    std::set<std::set<tuple<long long, long long, long long, long long>>> curSet = GetSetOf1();
//    auto curStep = 1;
//    while (true)
//    {
//        for (std::set<tuple<long long, long long, long long, long long>> removable : curSet)
//        {
//            if (IsRemovableValid(removable))
//            {
//                return removable;
//            }
//        }
//        cout << "\nCur step = " << curStep;
//        cout << "\n Size of Set = " << curSet.size();
//        ++curStep;
//        curSet = GetSmallerSubSetOfBots(curSet, curStep);
//        cout << "\t Next Step= " << curSet.size();
//    }
//}
//
//bool IsRemovableValid(std::set<tuple<long long, long long, long long, long long>>& removable)
//{
//    set<tuple<long long, long long, long long, long long>> allBots;
//    for (auto bot : bots)
//    {
//        allBots.insert(bot);
//    }
//    for (auto bot : removable)
//    {
//        allBots.erase(bot);
//    }
//    for (auto bot1 : allBots)
//    {
//        for (auto bot2 : allBots)
//        {
//            if (bot1 != bot2)
//            {
//                if (!DoSpheresOverlap(bot1, bot2))
//                    return false;
//            }
//        }
//    }
//    return true;
//}
//
//std::set<std::set<tuple<long long, long long, long long, long long>>> GetSetOf1()
//{
//    std::set<std::set<tuple<long long, long long, long long, long long>>> setOf1;
//    for (auto bot : bots)
//    {
//        std::set<tuple<long long, long long, long long, long long>> newerSet;
//        newerSet.insert(bot);
//        setOf1.insert(newerSet);
//    }
//    return setOf1;
//}
//
//std::set<std::set<tuple<long long, long long, long long, long long>>> GetSmallerSubSetOfBots(std::set<std::set<tuple<long long, long long, long long, long long>>>& setOfN, int N)
//{
//    std::set<std::set<tuple<long long, long long, long long, long long>>> setOfNPlus1;
//    for (auto subSet : setOfN)
//    {
//        for (auto bot : bots)
//        {
//            std::set<tuple<long long, long long, long long, long long>> newerSet = subSet;
//            subSet.insert(bot);
//            setOfNPlus1.insert(subSet);
//        }
//    }
//
//    std::set<std::set<tuple<long long, long long, long long, long long>>> removable;
//    for (std::set<tuple<long long, long long, long long, long long>> subSet : setOfNPlus1)
//    {
//        if (subSet.size() < N)
//        {
//            removable.insert(subSet);
//        }
//    }
//    for (auto subSet : removable)
//    {
//        setOfNPlus1.erase(subSet);
//    }
//
//    return setOfNPlus1;
//}
//
//void RemoveOrigin()
//{
//    std::set<tuple<long long, long long, long long, long long>> removable;
//    tuple<long long, long long, long long, long long> base = make_tuple(0, 0, 0, 0);
//    for (auto& item1 : bots)
//    {
//        if (IsSphereContained(item1, base))
//        {
//            removable.insert(item1);
//        }
//    }
//    for (auto item : removable)
//    {
//        bots.erase(item);
//    }
//}
//
//tuple<long long, long long, long long, long long> GetMostInRangeByCenters()
//{
//    auto point = make_tuple<long long, long long, long long, long long>(0, 0, 0, 0);
//    auto basePoint = point;
//    auto curMaxRange = 0;
//    auto curMaxPoint = make_tuple<long long, long long, long long, long long>(0, 0, 0, 0);
//    for (auto bot : bots)
//    {
//        auto explore = make_tuple(get<0>(bot), get<1>(bot), get<2>(bot), (long long)0);
//        auto range = GetInRange(explore);
//        if (range == curMaxRange)
//        {
//            auto d1 = GetManhattanDistance(basePoint, explore);
//            auto d2 = GetManhattanDistance(basePoint, curMaxPoint);
//            if (d1 < d2)
//            {
//                curMaxRange = range;
//                curMaxPoint = explore;
//            }
//        }
//        else if (range > curMaxRange)
//        {
//            curMaxRange = range;
//            curMaxPoint = explore;
//        }
//    }
//    cout << "\nANSWER = (" << get<0>(curMaxPoint) << "," << get<1>(curMaxPoint) << "," << get<2>(curMaxPoint) << ") \n";
//    cout << "\nRange = " << curMaxRange;
//    cout << "\n Distance from origin = " << GetManhattanDistance(basePoint, curMaxPoint);
//    get<3>(curMaxPoint) = 0;
//    Explore(curMaxPoint, curMaxRange);
//    return curMaxPoint;
//}
//
//void Explore(tuple<long long, long long, long long, long long> curMaxPoint, long long curMaxRange, long long step = 10000000)
//{
//    auto basePoint = make_tuple<long long, long long, long long, long long>(0, 0, 0, 0);
//    long long x, y, z, r;
//    tie(x, y, z, r) = curMaxPoint;
//    curMaxRange = 0;
//    for (auto i = x - step; i <= x + step; i += 10000000)
//    {
//        for (auto j = y - step; j <= y + step; j += 10000000)
//        {
//            for (auto k = z - step; k <= z + step; k += 10000000)
//            {
//                auto explore = make_tuple(i, j, k, (long long)0);
//                auto range = GetInRange(explore);
//                if (range == curMaxRange)
//                {
//                    auto d1 = GetManhattanDistance(basePoint, explore);
//                    auto d2 = GetManhattanDistance(basePoint, curMaxPoint);
//                    if (d1 < d2)
//                    {
//                        curMaxRange = range;
//                        curMaxPoint = explore;
//                    }
//                }
//                else if (range > curMaxRange)
//                {
//                    curMaxRange = range;
//                    curMaxPoint = explore;
//                }
//            }
//        }
//    }
//
//    cout << "\nANSWER = (" << get<0>(curMaxPoint) << "," << get<1>(curMaxPoint) << "," << get<2>(curMaxPoint) << ") \n";
//    cout << "\nRange = " << curMaxRange;
//    cout << "\n Distance from origin = " << GetManhattanDistance(basePoint, curMaxPoint);
//    if (step != 1)
//    {
//        Explore(curMaxPoint, curMaxRange, step / 10);
//    }
//}
//
//bool IsSphereContained(tuple<long long, long long, long long, long long> source, tuple<long long, long long, long long, long long> destination)
//{
//    long long x1, x2, x3, y1, y2, y3, z1, z2, z3, r1, r2;
//    tie(x1, y1, z1, r1) = source;
//    tie(x2, y2, z2, r2) = destination;
//    return (get<3>(source) >= GetManhattanDistance(source, destination));
//}
//
//int GetInRange(tuple<long long, long long, long long, long long> source)
//{
//    auto range = 0;
//    for (auto point : bots)
//    {
//        if (DoSpheresOverlap(source, point))
//        {
//            range++;
//        }
//    }
//    return range;
//}
//
//void MoveBySphere()
//{
//    auto point = make_tuple<long long, long long, long long, long long>(0, 0, 0, 0);
//    auto basePoint = point;
//    long long amplitude = 1024 * 1024 * 32;
//    auto curMaxRange = 0;
//    auto curMaxPoint = make_tuple<long long, long long, long long, long long>(0, 0, 0, 0);
//    do
//    {
//        for (auto x = get<0>(point) - amplitude; x <= get<0>(point); x += amplitude)
//        {
//            for (auto y = get<1>(point) - amplitude; y <= get<1>(point); y += amplitude)
//            {
//                for (auto z = get<2>(point) - amplitude; z <= get<2>(point); z += amplitude)
//                {
//                    auto explore = make_tuple(x, y, z, amplitude / 2);
//                    auto range = GetInRange(explore);
//                    if (range == curMaxRange)
//                    {
//                        auto d1 = GetManhattanDistance(basePoint, explore);
//                        auto d2 = GetManhattanDistance(basePoint, curMaxPoint);
//                        if (d1 < d2)
//                        {
//                            curMaxRange = range;
//                            curMaxPoint = explore;
//                        }
//                    }
//                    else if (range > curMaxRange)
//                    {
//                        curMaxRange = range;
//                        curMaxPoint = explore;
//                    }
//                }
//            }
//        }
//
//        if (point == curMaxPoint)
//        {
//            if (amplitude > 25000)
//            {
//                amplitude -= 1024;
//            }
//            else amplitude--;
//        }
//        point = curMaxPoint;
//        if (amplitude < 5)
//            cout << "\nRange = " << curMaxRange;
//        curMaxRange = 0;
//
//    } while (amplitude > 0);
//    cout << "\nANSWER = (" << get<0>(curMaxPoint) << "," << get<1>(curMaxPoint) << "," << get<2>(curMaxPoint) << ") \n";
//    cout << "\nRange = " << curMaxRange;
//    cout << "\n Distance from origin = " << GetManhattanDistance(basePoint, curMaxPoint);
//    auto s = GetMinPoint(bots);
//    cout << "\nANSWER = (" << get<0>(s) << "," << get<1>(s) << "," << get<2>(s) << ") \n";
//}
//
//std::set<tuple<long long, long long, long long, long long>> GetMostSpheres1()
//{
//    std::set<std::set<tuple<long long, long long, long long, long long>>> oldItems, newItems;
//    for (auto item : bots)
//    {
//        std::set<tuple<long long, long long, long long, long long>> s;
//        s.insert(item);
//        oldItems.insert(s);
//    }
//
//    bool hasInserted = true;
//    while (hasInserted)
//    {
//        hasInserted = false;
//        std::set<tuple<long long, long long, long long, long long>> pointsOfInterest;
//        for (auto& l : oldItems)
//        {
//            for (auto s : l)
//            {
//                pointsOfInterest.insert(s);
//            }
//        }
//
//        for (auto p : pointsOfInterest)
//        {
//
//            for (std::set<tuple<long long, long long, long long, long long>> l : oldItems)
//            {
//                if (l.find(p) == l.end() && DoSpheresOverlap(l, p))
//                {
//                    auto newSet = l;
//                    l.insert(p);
//                    newItems.insert(l);
//                    hasInserted = true;
//                }
//            }
//        }
//
//        if (hasInserted)
//        {
//            oldItems = newItems;
//            newItems.clear();
//        }
//    }
//
//    for (auto& l : oldItems)
//    {
//        auto s = GetMinPoint(l);
//        cout << "\nANSWER = (" << get<0>(s) << "," << get<1>(s) << "," << get<2>(s) << ") \n";
//    }
//
//    return *oldItems.begin();
//}
//
//tuple<long long, long long, long long> GetMinPoint(std::set<tuple<long long, long long, long long, long long>> spheres)
//{
//    auto first = *spheres.begin();
//    int x, y, z, r;
//    tie(x, y, z, r) = first;
//    pair<tuple<long long, long long, long long>, tuple<long long, long long, long long>> curRect =
//        make_pair(make_tuple(x - r, y - r, z - r), make_tuple(x + r, y + r, z + r));
//
//    for (auto sphere : spheres)
//    {
//        tie(x, y, z, r) = sphere;
//        pair<tuple<long long, long long, long long>, tuple<long long, long long, long long>> newRect =
//            make_pair(make_tuple(x - r, y - r, z - r), make_tuple(x + r, y + r, z + r));
//        curRect = GetOverlap(curRect, newRect);
//        /*cout << "(" << x << "," << y << "," << z << "), " << r <<" \n";
//        cout << "\t(" << x - r << "," << y - r << "," << z  - r<< "), " << r << " \n";
//        cout << "\t(" << x + r << "," << y + r << "," << z + r << "), " << r << " \n";*/
//    }
//
//    long long x1, x2, y1, y2, z1, z2;
//    tie(x1, y1, z1) = curRect.first;
//    tie(x2, y2, z2) = curRect.second;
//    cout << "(" << x1 << "," << y1 << "," << z1 << ") \n";
//    cout << "(" << x2 << "," << y2 << "," << z2 << ") \n";
//    for (auto i = 0; i <= x1; i++)
//    {
//        for (auto j = min(y1, y2); j <= max(y1, y2); j++)
//        {
//            for (auto k = min(z1, z2); k <= max(z2, z1); k++)
//            {
//                bool found = true;
//                for (auto s : spheres)
//                {
//                    auto dist = GetManhattanDistance(s, make_tuple(i, j, k, 0));
//                    if (GetManhattanDistance(s, make_tuple(i, j, k, 0)) > get<3>(s))
//                    {
//                        found = false;
//                        break;
//                    }
//                }
//                if (found)
//                {
//                    return make_tuple(i, j, k);
//                }
//            }
//        }
//    }
//}
//
//pair<tuple<long long, long long, long long>, tuple<long long, long long, long long>> GetOverlap(
//    pair<tuple<long long, long long, long long>, tuple<long long, long long, long long>> first,
//    pair<tuple<long long, long long, long long>, tuple<long long, long long, long long>> second
//)
//{
//    long long x1, x2, x3, x4, y1, y2, y3, y4, z1, z2, z3, z4;
//    tie(x1, y1, z1) = first.first;
//    tie(x2, y2, z2) = first.second;
//    tie(x3, y3, z3) = second.first;
//    tie(x4, y4, z4) = second.second;
//
//    return make_pair(make_tuple(max(x1, x3), max(y1, y3), max(z1, z3)), make_tuple(min(x4, x2), min(y4, y2), min(z4, z2)));
//}
//
//bool DoSpheresOverlap(std::set<tuple<long long, long long, long long, long long>>& spheres, tuple<long long, long long, long long, long long> target)
//{
//    for (auto& s : spheres)
//    {
//        if (!DoSpheresOverlap(s, target))
//        {
//            return false;
//        }
//    }
//    return true;
//}
//
//int GetMaxRange()
//{
//
//    auto maxR = numeric_limits<int>::min();
//    tuple<long long, long long, long long, long long> maxItem = make_tuple(0, 0, 0, 0);
//
//    for (auto item : bots)
//    {
//        if (get<3>(item) > maxR)
//        {
//            maxR = get<3>(item);
//            maxItem = item;
//        }
//    }
//    auto range = 0;
//    for (auto item : bots)
//    {
//        if (GetManhattanDistance(item, maxItem) <= maxR)
//        {
//            range++;
//        }
//    }
//
//    return range;
//}
//
