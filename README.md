*To Compile*
- I used gcc version 12.1.0 supporting c++20
  .....
1. open terminal and navigate to the project directory.
2. compile using the command: `g++ -std=c++20 -o main main.cpp`.
3. Run the executable

*[1] Problems Identified*

- iostream header not included
- std::optional not included
- remove() and remove_if() not members of <ranges> therefore included std::algorithm - ref https://en.cppreference.com/w/cpp/algorithm/ranges/remove
![image](https://github.com/user-attachments/assets/12c2a79c-6f56-4122-b799-12c5479f8771)

*[2] Implementing Phantom Power Feature*
- added a boolean member to device class - "phantomPowerStatus"
- added setter and getter (as string to be displayed - for the sake of clarity for the user)
  ![image](https://github.com/user-attachments/assets/52c8ffa4-78ed-43e9-8b58-1275651ff0dd)
- inside runApp() method added set-phantom-power to list of commands displayed to user
- also added line to show phantom-power when user runs the command 'status'
- processed command input by user inside processDeviceCommand() method
 ![image](https://github.com/user-attachments/assets/a025bac2-892e-4896-bcee-6800d7f8aeab)


*[3] Ensuring Preamp level in range*
- just a simple if, else statement to check - inside setPreampLevel method
- also added try catch block inside processCommand to ensure non-numerical values for preamp values are handled


  ![image](https://github.com/user-attachments/assets/b168762c-1a1e-42db-b5aa-865daf31d5ff)

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*Screenshot of Application*
![image](https://github.com/user-attachments/assets/a8b29ef9-d795-45d9-808e-064269b99236)
