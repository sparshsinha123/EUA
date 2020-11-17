#ifndef EDGE_USER
#define EDGE_USER
#define ld long double
#include <string>
class edgeServer{
	public:
	int id;
	ld latitude;
	ld longitude;
	ld cpu;
	ld ram;
	ld storage;
	ld bandwidth;

  public:
	edgeServer (int id, ld latitude, ld longitude, ld cpu, ld ram, ld storage, ld bandwidth){
		this->id = id;
		this->latitude = latitude;
		this->longitude = longitude;
		this->cpu = cpu;
		this->ram = ram;
		this->storage = storage;
		this->bandwidth = bandwidth;
	}
  bool operator<(const edgeServer& t) const{ 
        return (this->cpu + this->ram + this->storage + this->bandwidth < 
                t.cpu + t.ram + t.storage + t.bandwidth); 
  } 
	void printDetails (){
		std::cout << "<serverId: " << id << ", latitude: " << latitude << ", longitude: " << longitude;
		std::cout << ", cpu: " << cpu << ", ram: " << ram << ", storage: " << storage << ", bandwidth: ";
		std::cout << bandwidth << ">\n";
	}
};

class user{
	public:
	int id;
	ld latitude;
	ld longitude;
	ld cpu;
	ld ram;
	ld storage;
	ld bandwidth;
    
  public:
	user (int id, ld latitude, ld longitude, ld cpu, ld ram, ld storage, ld bandwidth){
		this->id = id;
		this->latitude = latitude;
		this->longitude = longitude;
		this->cpu = cpu;
		this->ram = ram;
		this->storage = storage;
		this->bandwidth = bandwidth;
	}
  bool operator< (const user &t) const{
    return (this->cpu + this->ram + this->storage + this->bandwidth <
            t.cpu + t.ram + t.storage + t.bandwidth);
  }
	void printDetails (){
		std::cout << "<userId: " << id << ", latitude: " << latitude << ", longitude: " << longitude;
		std::cout << ", cpu: " << cpu << ", ram: " << ram << ", storage: " << storage << ", bandwidth: ";
		std::cout << bandwidth << ">\n";
	}
};

#endif