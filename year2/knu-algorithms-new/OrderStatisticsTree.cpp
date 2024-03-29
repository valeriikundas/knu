#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "redblacktree.h"
#include "OrderStatisticsTree.h"

using namespace std;

string random_string(int n = 2) {
  string res = "";
  for (int i = 0; i < n; ++i)
    res += ('a' + rand() % 26);
  return res;
}

vector<string> generate_strings(int n = 2) {
  vector<string> res;
  for (int i = 0; i < n; ++i)
    res.push_back(random_string());
  return res;
}

int main() {
  ofstream log;
  log.open("log.txt");
  srand((int)time(0));
  RedBlackTree<string, vector<string>> warehouse_by_section;
  RedBlackTree<string, string> warehouse_by_product;
  RedBlackTree<string, string> t;
  while (true) {
    cout << "1 - insert a section\n2 - delete a section\n3 - find section index\n4 - check if section exists\n5 - insert a product\n6 - delete a product\n7 - check if product exists\n8 - find product index\n9 - exit\n";
    int ch;
    cin >> ch;
    vector<string> products;
    string section, product;
    switch (ch) {
      case 1:
      {
        cout << "enter section to insert: ";
        cin >> section;
        products = generate_strings();
        warehouse_by_section.insert(section, products);

        for (int i = 0; i < (int)products.size(); ++i)
          warehouse_by_product.insert(products[i], section);
        break;
      }
      case 2:
      {
        cout << "enter section to remove: ";
        string section;
        cin >> section;
        products = warehouse_by_section.get_value(section);
        warehouse_by_section.remove(section);
        for (int i = 0; i < (int)products.size(); ++i)
          warehouse_by_product.remove(products[i]);
        break;
      }
      case 3:
      {
        cout << "enter section title: ";
        cin >> section;
        cout << warehouse_by_section.find_index(section) << "\n";
        break;
      }
      case 4:
      {
        cout << "enter section title: ";
        cin >> section;
        cout << (warehouse_by_section.contains(section) ? "exists" : "not exists") << "\n";
        break;
      }
      case 5:
      {
        cout << "enter a product and a section: ";
        cin >> product >> section;
        if (warehouse_by_section.contains(section)) {
          cout << "such section already exists\n";
        }
        else {
          warehouse_by_section.insert(section, { product });
          warehouse_by_product.insert(product, section);
        }
        break;
      }
      case 6:
      {
        cout << "enter a product title: ";
        cin >> product;
        warehouse_by_product.remove(product);
        break;
      }
      case 7:
      {
        cout << "enter a product title: ";
        cin >> product;
        cout << (warehouse_by_product.contains(product) ? "exists" : "not exists") << "\n";
        break;
      }
      case 8:
      {
        cout << "enter a product title: ";
        cin >> product;
        cout << warehouse_by_product.find_index(product) << "\n";
        break;
      }
      default:
      {
        return 0;
      }
    }
    warehouse_by_section.show_by_section();
    cout << "\n";
    warehouse_by_product.show();
    cout << "\n\n";
  }
}
