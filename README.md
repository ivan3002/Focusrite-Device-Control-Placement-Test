*To Compile*
- I used gcc version 12.1.0
- and ran
g++ -std=c++20 -o main main.cpp

*Problems Identified*

- iostream header not included
- std::optional not included
- remove() and remove_if() not members of <ranges> therefore included <algorithm>
![image](https://github.com/user-attachments/assets/12c2a79c-6f56-4122-b799-12c5479f8771)

