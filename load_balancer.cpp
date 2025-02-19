#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
// etc.

using namespace std;

class Server {
private:
    int currentLoad;
    string ipAddress;
public:
    Server(string ipAddress, int currentLoad) : ipAddress(ipAddress), currentLoad(currentLoad) {
        // constructor
    }

    string getIpAddress() {
        return ipAddress;
    }
    int getLoad() {
        return currentLoad;
    }

    void incrementLoad() {
        currentLoad++;
    }
};

class LoadBalancer {
private:
    vector<Server> servers;
public:
    void addServer(string ip, int load) {
        servers.push_back(Server(ip, load));
    }

    string getServer() {
        //get server with least load
        int minn = INT_MAX;
        int min_ind = 0;
        for(int i = 0; i<servers.size(); ++i) {
            if (servers[i].getLoad() < minn) {
                minn = servers[i].getLoad();
                min_ind = i;
            }
        }
        servers[min_ind].incrementLoad();
        return servers[min_ind].getIpAddress();
    }
};

int main() {
    LoadBalancer lb;
    lb.addServer("192.168.1.10", 10000);
    lb.addServer("192.168.1.11", 10);
    lb.addServer("192.168.1.14", 10);
    
    cout << "Request routed to server: " << lb.getServer() << endl;
    cout << "Request routed to server: " << lb.getServer() << endl;
    return 0;

}