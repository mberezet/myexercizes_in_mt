# myexercizes_in_mt

//compile with -pthread compile opton
//   g++ -std=c++17 [-g] [-On] [-pthread]  [-lpthread | -lm]


// install g++ 7 to ubuntu [c++17 support by deflt]
sudo apt-get update -y && \
sudo apt-get upgrade -y && \
sudo apt-get dist-upgrade -y && \
sudo apt-get install build-essential software-properties-common -y && \
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y && \
sudo apt-get update -y && \
sudo apt-get install gcc-7 g++-7 -y && \
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 60 --slave /usr/bin/g++ g++ /usr/bin/g++-7 && \
sudo update-alternatives --config gcc
