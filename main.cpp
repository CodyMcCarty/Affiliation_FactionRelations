#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <math.h>
#include <functional>
#include <string>

// using namespace std;

struct IntPairHash
{
    std::size_t operator()(const std::pair<int, int> &p) const
    {
        return std::hash<int>()(p.first) ^ std::hash<int>()(p.second);
    }
};

class FactionRelationships
{
public:
    FactionRelationships() : relationships{}
    {
        // initialize all relationships as neutral and avoids duplicates.  Use Print() to visualize.
        for (int i = 0; i < NumOfFactionsAtStart; i++)
        {
            for (int j = i; j < NumOfFactionsAtStart; j++)
            {
                if (i == j)
                {
                    relationships[{i, j}] = 1.0f;
                }
                else
                {
                    relationships[{i, j}] = 0.5f;
                }
            }
        }
    }

    // returns -1.0f if relationship doesn't exist
    float GetRelationship(int faction1, int faction2) const
    {
        if (faction1 == faction2)
            return 1.0f;

        int a, b;
        std::tie(a, b) = std::minmax(faction1, faction2);
        auto it = relationships.find(std::make_pair(a, b));

        if (it == relationships.end())
        {
            // TODO: log warning
            return -1.0f;
        }

        return it->second;
    }

    std::unordered_map<std::pair<int, int>, float, IntPairHash> GetRelationships() const
    {
        return relationships;
    }

    int GetLastFaction()
    {
        int lastFaction = 0;
        for (const auto &entry : relationships)
        {
            lastFaction = std::max({lastFaction, entry.first.first, entry.first.second});
        }
        return lastFaction + 1;
    }

    void CreateNewFaction(int faction, float standing = 0.5f)
    {
        for (auto it = relationships.begin(); it != relationships.end();)
        {
            if (it->first.first == faction || it->first.second == faction)
            {
                std::cout << "Faction already exist" << std::endl;
                return;
            }
        }

        for (int i = 0; i < relationships.size(); i++)
        {
            int a, b;
            std::tie(a, b) = std::minmax(i, faction);
            if (a == b)
            {
                relationships[{a, b}] = 1.0f;
            }
            else
            {
                relationships[{a, b}] = standing;
            }
        }
    }

    // used to set a relationship.  See UpdateRelationship() to increment a relationship.
    void SetRelationship(int faction1, int faction2, float standing = 0.5f)
    {
        if (faction1 == faction2)
        {
            relationships[{faction1, faction2}] = 1.0f;
        }

        int a, b;
        std::tie(a, b) = std::minmax(faction1, faction2);
        if (relationships.find(std::make_pair(a, b)) == relationships.end())
        {
            std::cout << "Relationship does not exist" << std::endl;
        }
        else
        {
            relationships[{a, b}] = standing;
        }
    }

    // increments or decrements the faction's relations.
    void UpdateRelationship(int faction1, int faction2, float incrementStandingBy = -0.1f)
    {
        int a, b;
        std::tie(a, b) = std::minmax(faction1, faction2);
        auto key = std::make_pair(a, b);
        if (relationships.find(key) == relationships.end())
        {
            std::cout << "Relationship does not exist" << std::endl;
        }
        else
        {
            relationships[key] += incrementStandingBy;
            // clamp the relationship value to between 0 and 1
            relationships[{a, b}] = std::max(0.0f, std::min(1.0f, relationships[{a, b}]));
        }
    }

    // set relations to -1.0f
    void SoftDelFaction(int faction)
    {
        for (auto it = relationships.begin(); it != relationships.end();)
        {
            if (it->first.first == faction || it->first.second == faction)
            {
                it->second = -1.0f;
            }
            else
            {
                ++it;
            }
        }
    }

    // Erases faction.  See SoftDelFaction() to set relations to -1.
    void DeleteFaction(int faction)
    {
        for (auto it = relationships.begin(); it != relationships.end();)
        {
            if (it->first.first == faction || it->first.second == faction)
            {
                it = relationships.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    // prints a human readable table
    void GetRelationshipsTable() const
    {
        int w = 6;
        // Determine the maximum faction ID so we know how wide to make the table
        int maxFactionId = NumOfFactionsAtStart;
        for (const auto &[key, value] : relationships)
        {
            maxFactionId = std::max(maxFactionId, key.first);
            maxFactionId = std::max(maxFactionId, key.second);
        }

        // Calculate the column and row headers
        std::cout << std::setw(w) << ""
                  << "|";
        for (int i = 1; i <= maxFactionId; i++)
        {
            std::cout << std::setw(w) << i << "|";
        }
        std::cout << "\n";

        // Print the horizontal separator
        std::cout << std::setfill('-') << std::setw(5 + 5 * maxFactionId) << "\n"
                  << std::setfill(' ');

        // Print the table contents
        for (int i = 1; i <= maxFactionId; i++)
        {
            if (i % 5 == 0)
                std::cout << std::endl;
            std::cout << std::setw(w) << i << "|";
            for (int j = 1; j <= maxFactionId; j++)
            {
                auto relationship = relationships.find(std::make_pair(i, j));
                if (relationship != relationships.end())
                {
                    std::cout << std::fixed << std::setprecision(2) << std::setw(w) << relationship->second << "|";
                }
                else
                {
                    std::cout << std::setw(w) << ""
                              << "|";
                }
            }
            std::cout << "\n";
        }
    }

    // Add this public member function to enable iteration over the relationships
    std::unordered_map<std::pair<int, int>, float>::iterator begin() { return relationships.begin(); }
    std::unordered_map<std::pair<int, int>, float>::iterator end() { return relationships.end(); }

private:
    std::unordered_map<std::pair<int, int>, float, IntPairHash> relationships;

    float NumOfFactionsAtStart = 12;

    void MinMax(int faction1, int faction2, int &min, int &max) const
    {
        min = std::min(faction1, faction2);
        max = std::max(faction1, faction2);
    }
};

class Test
{
public:
    void Get(int faction1, int faction2, float expected, std::function<float(int, int)> func, int &testNum, std::string desc = "-")
    {
        testNum++;
        float rlt = func(faction2, faction1);
        float tolerance = 0.001f;
        bool bR = equals(expected, rlt);
        std::cout << testNum << (bR ? " pass " : " fail ") << desc << std::endl;
        if (!bR)
        {
            std::cout << "E: " << expected << "\nA: " << rlt << std::endl;
        }
        std::cout << std::endl;
    }

private:
    bool equals(float a, float b, float tolerance = 0.0001f)
    {
        return (a - b < tolerance) && (b - a < tolerance);
    }
};

int main()
{
    FactionRelationships factionRelationships;
    Test test;
    int num = 9;

    std::cout
        << "\nGetRelationship " << num << "\n"
        << std::endl;
    test.Get(
        7, 8, 0.5f, [&](int a, int b)
        { return factionRelationships.GetRelationship(a, b); },
        num);

    test.Get(
        8, 7, 0.5f, [&](int a, int b)
        { return factionRelationships.GetRelationship(a, b); },
        num);

    test.Get(
        7, 7, 1.0f, [&](int a, int b)
        { return factionRelationships.GetRelationship(a, b); },
        num);

    test.Get(
        70, 7, -1.0f, [&](int a, int b)
        { return factionRelationships.GetRelationship(a, b); },
        num);

    // factionRelationships.DeleteFaction(3);

    // float x = factionRelationships.GetRelationship(3, 2);
    // std::cout << x << std::endl;

    // factionRelationships.PrintTable();

    // // test.Get(0.5f, 7, 8, [&](int a, int b) { return factionRelationships.GetRelationship(a, b); }, num);
    // // test.Get(1.0f, 7, 7, [&](int a, int b) { return factionRelationships.GetRelationship(a, b); }, num);

    //   // set some example relationships
    //   factionRelationships.SetRelationship(2, 3, 0.1f);
    //   factionRelationships.SetRelationship(2, 4, 0.2f);
    //   factionRelationships.SetRelationship(2, 5, 0.8f);
    //   factionRelationships.SetRelationship(6, 2, 0.0f);

    // // factionRelationships.PrintTable();

    //   // test the Relationship function
    // test.Get(2, 3, 0.1f, [&](int a, int b) { return factionRelationships.GetRelationship(a, b); }, num);
    // test.Get(3, 2, 0.1f, [&](int a, int b) { return factionRelationships.GetRelationship(a, b); }, num);
    // test.Get(2, 4, 0.2f, [&](int a, int b) { return factionRelationships.GetRelationship(a, b); }, num);
    // test.Get(5, 2, 0.8f, [&](int a, int b) { return factionRelationships.GetRelationship(a, b); }, num);
    // test.Get(6, 2, 0.0f, [&](int a, int b) { return factionRelationships.GetRelationship(a, b); }, num);
    // test.Get(60, 2, 0.1f, [&](int a, int b) { return factionRelationships.GetRelationship(a, b); }, num, ":should fail and cout out");

    // // set update examples
    // factionRelationships.UpdateRelationship(7, 8, -0.3);
    // factionRelationships.UpdateRelationship(10, 8, 0.3);
    // factionRelationships.UpdateRelationship(9, 8);

    // test.Get(7, 8, (0.5-0.3), [&](int a, int b) { return factionRelationships.GetRelationship(a, b); }, num);
    // test.Get(10, 8, (0.5+0.3), [&](int a, int b) { return factionRelationships.GetRelationship(a, b); }, num);
    // test.Get(9, 8, 0.4, [&](int a, int b) { return factionRelationships.GetRelationship(a, b); }, num);

    // factionRelationships.UpdateRelationship(10, 8, 0.5);
    // test.Get(10, 8, (1.0), [&](int a, int b) { return factionRelationships.GetRelationship(a, b); }, num);

    // factionRelationships.UpdateRelationship(7, 8, -0.5);
    // test.Get(7, 8, 0.0, [&](int a, int b) { return factionRelationships.GetRelationship(a, b); }, num);

    // factionRelationships.UpdateRelationship(2, 4, -0.5);
    // test.Get(2, 4, 0.0, [&](int a, int b) { return factionRelationships.GetRelationship(a, b); }, num);

    // factionRelationships.UpdateRelationship(2, 4, 0.5);
    // factionRelationships.UpdateRelationship(2, 4, 0.4);
    // test.Get(2, 4, 0.9, [&](int a, int b) { return factionRelationships.GetRelationship(a, b); }, num);

    return 0;
}
