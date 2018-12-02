#include "stdafx.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <array>
#include <ctime>
#include <map>

#include "perfecthashing.h"

using namespace std;

int main() {
  srand((int)time(0));

  map<string, vector<string>> all_products = {
    {"napitki", {"cola", "sprite", "fanta"}},
    {"molochka", {"yogurt", "moloko", "syr"}},
    {"myaso", {"telyatina", "kuritsa", "svinina"}}
  };

  vector<pair<string, vector<string>>> sections;
  for (auto iter : all_products) {
    auto section = iter.first;
    sections.push_back(make_pair(iter.first, iter.second));
  }

  vector<pair<string, string>> products;
  for (int i = 0; i < (int)sections.size(); ++i) {
    for (int j = 0; j < (int)sections[i].second.size(); ++j) {
      products.push_back(make_pair(sections[i].second[j], sections[i].first));
    }
  }

  for (int i = 0; i < (int)sections.size(); ++i) {
    cout << sections[i].first << ": ";
    for (int j = 0; j < (int)sections[i].second.size(); ++j) {
      cout << sections[i].second[j] << ",";
    }
    cout << "\n";
  }
  cout << "\n";

  for (int i = 0; i < (int)products.size(); ++i) {
    cout << products[i].first << " " << products[i].second << "\n";
  }
  cout << "\n";
  system("pause");

  FirstLevelHashTable<string, string> warehouse_by_product(products);
  FirstLevelHashTable<string, vector<string>> warehouse_by_section(sections);

  while (true) {
    cout << "1-view product, 2-view section, 3-exit\n";
    int ch;
    cin >> ch;
    string product, section;
    switch (ch) {
      case 1:
      {
        cin >> product;
        cout << warehouse_by_product.view(product) << "\n";
        break;
      }
      case 2:
      {
        cin >> section;
        vector<string> products = warehouse_by_section.view(section);
        for (int i = 0; i < (int)products.size(); ++i)
          cout << products[i] << " ";
        cout << "\n";
        break;
      }
      default:
      {
        return 0;
      }
    }
    warehouse_by_product.show();
    cout << "\n";
    warehouse_by_section.show_by_section();
    cout << "\n\n";
  }

  /*vector<pair<int, int>> b;
  for (int i = 0; i < 5; ++i)
    for (int j = 0; j < 10; ++j)
      b.push_back(make_pair(i, rand() % 20));

  FirstLevelHashTable<int, int> a(b);

  a.show();*/

  return 0;
}
