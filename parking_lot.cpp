#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

// Ticket class to store ticket details
class Ticket {
private:
    int ticket_id;
    int spot_id;
    time_t time_of_entry;
    time_t time_of_exit;
    int cost;
    string vehicle_number;

public:
    Ticket(int ticket_id, int spot_id, string vehicle_number) 
        : ticket_id(ticket_id), spot_id(spot_id), vehicle_number(vehicle_number) {
        time_of_entry = time(nullptr); // Store current time
        cost = 0;
    }

    int getSpotId() {
        return spot_id;
    }

    void print_ticket(time_t exit_time) {
        time_of_exit = exit_time;
        int duration = difftime(time_of_exit, time_of_entry) / 60; // Convert to minutes
        cost = duration * 2; // Assume $2 per minute rate

        cout << "\n------ Parking Ticket ------\n";
        cout << "Ticket ID: " << ticket_id << "\n";
        cout << "Vehicle Number: " << vehicle_number << "\n";
        cout << "Spot ID: " << spot_id << "\n";
        cout << "Entry Time: " << ctime(&time_of_entry);
        cout << "Exit Time: " << ctime(&time_of_exit);
        cout << "Total Cost: $" << cost << "\n";
        cout << "----------------------------\n";
    }
};

// Parking Spot class
class ParkingSpots {
private:
    int spot_id;
    bool is_occupied;
    string vehicle_number;

public:
    ParkingSpots() : is_occupied(false), vehicle_number("") {}

    void addVehicle(int id, string vehicle_num) {
        spot_id = id;
        vehicle_number = vehicle_num;
        is_occupied = true;
    }

    void removeVehicle() {
        is_occupied = false;
        vehicle_number = "";
    }

    bool isFree() {
        return !is_occupied;
    }
};

// Parking Lot class
class ParkingLot {
private:
    int total_spots;
    vector<ParkingSpots> spots;
    int ticket_counter;

public:
    ParkingLot(int total_spots) : total_spots(total_spots), ticket_counter(0) {
        spots = vector<ParkingSpots>(total_spots);
    }

    int getFreeSpot() {
        for (int i = 0; i < total_spots; ++i) {
            if (spots[i].isFree()) {
                return i;
            }
        }
        return -1; // No Free Spots
    }

    Ticket* assignSpot(string vehicle_num) {
        int spot_id = getFreeSpot();
        if (spot_id == -1) {
            cout << "No Free Spot Available!\n";
            return nullptr;
        }

        spots[spot_id].addVehicle(spot_id, vehicle_num);
        ticket_counter++;
        cout << "Vehicle " << vehicle_num << " assigned to spot " << spot_id << ".\n";

        return new Ticket(ticket_counter, spot_id, vehicle_num); // Returning dynamically allocated Ticket
    }

    void removeSpot(Ticket* ticket) {
        if (!ticket) return;

        int spotId = ticket->getSpotId();
        spots[spotId].removeVehicle();
        ticket->print_ticket(time(nullptr)); // Print exit time

        delete ticket; // Free memory
    }
};

// Main function
int main() {
    ParkingLot lot(5); // Create a parking lot with 5 spots

    // Assign spots
    Ticket* ticket1 = lot.assignSpot("ABC123");
    Ticket* ticket2 = lot.assignSpot("XYZ456");

    // Simulate delay (for testing time difference)
    cout << "Simulating parking duration...\n";
    //this_thread::sleep_for(chrono::seconds(5)); // 5 seconds delay

    // Remove spots (exit)
    lot.removeSpot(ticket1);
    lot.removeSpot(ticket2);

    return 0;
}
