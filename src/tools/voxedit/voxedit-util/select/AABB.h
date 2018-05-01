/**
 * @file
 */

#pragma once

#include "Select.h"

namespace voxedit {
namespace selections {

class AABB : public Select {
private:
	glm::ivec3 _mins {0};
	glm::ivec3 _maxs {0};
	bool _started = false;

public:
	SelectionSingleton(AABB)

	void unselect() override;
	int execute(voxel::RawVolume::Sampler& model, voxel::RawVolume::Sampler& selection) override;
};

}
}
