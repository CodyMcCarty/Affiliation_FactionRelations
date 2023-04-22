#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <math.h>
#include <functional>

using namespace std;

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
        // initialize all relationships as neutral and avoids duplicates
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
        MinMax(faction1, faction2, a, b);
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
                cout << endl;
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

    void CreateRelationship(int faction1, int faction2, float standing = 0.5f) // just take one faction
    {
        int a, b;
        MinMax(faction1, faction2, a, b);
        auto key = make_pair(a, b);
        if (relationships.find(key) != relationships.end())
        {
            cout << "Relationship already exists" << endl;
        }
        else
        {
            relationships[key] = standing;
        }
    }

    void UpdateRelationship(int faction1, int faction2, float standing = 0.5f)
    {
        int a, b;
        MinMax(faction1, faction2, a, b);
        auto key = make_pair(a, b);
        if (relationships.find(key) == relationships.end())
        {
            cout << "Relationship does not exist" << endl;
        }
        else
        {
            relationships[key] = standing;
        }
    }

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

    // float GetRelationship(int faction1, int faction2)
    // {
    //     if (faction1 >= NumOfFactionsAtStart || faction2 >= NumOfFactionsAtStart)
    //     {
    //       //TODO: print warning
    //       cout << "out of Bounds" << endl;
    //       return 0.0f;
    //     }

    //     int a, b;
    //     MinMax(faction1, faction2, a, b);
    //     return relationships[{a, b}];
    // }

    // void SetRelationship(int faction1, int faction2, float value)
    // {
    //   if (faction1 == faction2)
    //   {
    //     relationships[{faction1, faction2}] = 1.0f;
    //   }

    //   int a, b;
    //   MinMax(faction1, faction2, a, b);
    //   relationships[{a, b}] = value;
    // }

    // void UpdateRelationship (int faction1, int faction2, float amount = -0.1f)
    // {
    //   int a, b;
    //   MinMax(faction1, faction2, a, b);
    //   relationships[{a, b}] += amount;
    //   // clamp the relationship value to between 0 and 1
    //   relationships[{a, b}] = std::max(0.0f, std::min(1.0f, relationships[{a, b}]));
    // }

    // void CreateRelationship(int faction1, int faction2, float standing = 0.5)
    // {
    //   // if (faction1 <= )
    //   // int a, b;
    //   // MinMax(faction1, faction2, a, b);
    // }

    // Add this public member function to enable iteration over the relationships
    std::unordered_map<std::pair<int, int>, float>::iterator begin() { return relationships.begin(); }
    std::unordered_map<std::pair<int, int>, float>::iterator end() { return relationships.end(); }

    void PrintTable()
    {
        double n = 1;
        int colWidth = 4;
        // Table Heder
        std::cout << setw(colWidth) << " ";
        for (int i = 0; i < NumOfFactionsAtStart; i++)
        {
            std::cout << setw(colWidth) << i;
        }

        // print body
        for (int i = 0; i < NumOfFactionsAtStart; i++)
        {
            std::cout << endl;
            if (i % 5 == 0)
            {
                cout << "\n";
            }
            std::cout << setw(colWidth) << i;

            for (int j = 0; j < NumOfFactionsAtStart; j++)
            {
                if (relationships[{i, j}] == 1.0f)
                {
                    cout << setw(colWidth) << "1.0";
                }
                else if (!relationships[{i, j}])
                {
                    cout << setw(colWidth) << "  ";
                }
                else if (relationships[{i, j}] == 0)
                {
                    cout << setw(colWidth) << "0.0";
                }
                else
                {
                    cout << setprecision(2) << setw(colWidth) << relationships[{i, j}];
                }
            }
        }

        // end space
        std::cout << "\n"
                  << std::endl;
    }

private:
    std::unordered_map<std::pair<int, int>, float, IntPairHash> relationships;

    float NumOfFactionsAtStart = 12;

    // pair<int, int> key(20, 4);
    // auto it = factionRelationships.relationships.find(key);
    // if (it != factionRelationships.relationships.end())
    // {
    //   cout << "exist, is: " << it->second << endl;
    // }
    // else
    // {
    //   cout << "nope " << endl;
    // }

    void MinMax(int faction1, int faction2, int &min, int &max) const
    {
        min = std::min(faction1, faction2);
        max = std::max(faction1, faction2);
    }
};

class Test
{
public:
    void Get(int faction1, int faction2, float expected, std::function<float(int, int)> func, int &testNum, string desc = "-")
    {
        testNum++;
        float rlt = func(faction2, faction1);
        float tolerance = 0.001f;
        bool bR = equals(expected, rlt);
        cout << testNum << (bR ? " pass " : " fail ") << desc << endl;
        if (!bR)
        {
            cout << "E: " << expected << "\nA: " << rlt << endl;
        }
        cout << endl;
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

    factionRelationships.DeleteFaction(3);

    float x = factionRelationships.GetRelationship(3, 2);
    cout << x << endl;

    factionRelationships.GetRelationshipsTable();

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
