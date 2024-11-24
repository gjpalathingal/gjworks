// Find intersection of 2 arrays with O(n)

#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

int main()
{
    unsigned *arr1, *arr2;
    unsigned count1, count2;
    unordered_map <unsigned, unsigned> m1, m2, m3;
    vector<unsigned> intersection;

    std::cout << "Enter no of elements in array1: ";
    std::cin>> count1;
    std::cout << endl << "Enter the elements";
    arr1 = new unsigned[count1];
    for (unsigned i = 0; i < count1 ; i++)
    {
        std::cin >> arr1[i];
    }

    std::cout << "Enter no of elements in array1: ";
    std::cin>> count2;
    std::cout << endl << "Enter the elements";
    arr2 = new unsigned[count2];
    for (unsigned i = 0; i < count2 ; i++)
    {
        std::cin >> arr2[i];
    }

    for (unsigned i = 0; i < count1; i++)
    {
        if (m1.find(arr1[i]) == m1.end())
        {
            m1[arr1[i]] = 1;
        }
        else
        {
            m1[arr1[i]] += 1;
        }
    }

    for (unsigned i = 0; i < count2; i++)
    {
        if (m2.find(arr2[i]) == m2.end())
        {
            m2[arr2[i]] = 1;
        }
        else
        {
            m2[arr2[i]] += 1;
        }
    }

    for (auto i : m1)
    {
        if (m2.find(i.first) != m2.end())
        {
            unordered_map<unsigned, unsigned>::iterator it;
            it = m2.find(i.first);
            m3[i.first] = min(i.second, it->second);
        }
    }

    if (m3.size() != 0)
    {
        unordered_map<unsigned, unsigned>::iterator it;
        for (it = m3.begin(); it != m3.end(); it++)
        {
            for (unsigned j = 0; j < it->second; j++)
            {
                std::cout << it->first << " ";
            }
        }
    }

    return 0;
}
