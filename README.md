*To Compile*
- I used gcc version 12.1.0 supporting c++20
- and ran
"g++ -std=c++20 -o main main.cpp"

*[1] Problems Identified*

- iostream header not included
- std::optional not included
- remove() and remove_if() not members of <ranges> therefore included <algorithm>
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
  ![image](https://github.com/user-attachments/assets/b168762c-1a1e-42db-b5aa-865daf31d5ff)

