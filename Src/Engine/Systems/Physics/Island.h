#include "RigidBody.h"
#include <vector>
//TODO:: collision solving and applying of forces/torques/velocities here
struct VelocitySaved {
	glm::vec3 w;
	glm::vec3 v;
};

class Island {
	std::vector < std::shared_ptr<RigidBody>> i_bodies;
	int i_bodyCount;
	int i_maxBodyCount;
	bool i_friction;
	VelocitySaved* i_velocities;
public:	
	void addBody(std::shared_ptr<RigidBody> body);
	void initialize();
	void solve();
};

