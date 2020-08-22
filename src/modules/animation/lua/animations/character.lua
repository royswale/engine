local boneutil = require 'animations.boneutil'
local chr = require 'animations.characterskeleton'

local c_halfPi = math.pi/ 2.0

function swim(animTime, v, skeleton, skeletonAttr)
	local timeFactor = skeletonAttr.runTimeFactor
	local sin = math.sin
	local cos = math.cos
	local scaledTime = animTime * timeFactor;
	local sine = sin(scaledTime)
	local cosine = cos(scaledTime)
	local cosineSlow = cos(0.25 * scaledTime)
	local movement = 0.15 * sine
	local animTimeCos = cos(animTime);
	local headLookX = math.rad(-30.0) + 0.1 * animTimeCos;
	local headLookY = 0.1 * sine;
	local velocity = boneutil.clamp(0.05 * v, 0.1, 2.5)

	local head = skeleton:bone("head")
	head.scale = vec3.new(skeletonAttr.headScale)
	head.translation = vec3.new(0.0, 0.5 + skeletonAttr.neckHeight + skeletonAttr.headY + 1.3 * cosine, -1.0 + skeletonAttr.neckForward)
	head.orientation = boneutil.rotateXY(headLookX, headLookY)

	local bodyMoveY = cosine * 0.5
	local chest = chr.chestBone(skeleton, skeletonAttr)
	chest.translation = vec3.new(0.0, skeletonAttr.chestY + bodyMoveY, 0.0)
	chest.orientation = boneutil.rotateY(movement)

	local belt = chr.beltBone(skeleton, skeletonAttr)
	belt.translation = vec3.new(0.0, skeletonAttr.beltY + bodyMoveY, 0.0)
	belt.orientation = boneutil.rotateY(movement)

	local pants = chr.pantsBone(skeleton, skeletonAttr)
	pants.translation = vec3.new(0.0, skeletonAttr.pantsY + bodyMoveY, 0.0)
	pants.orientation = boneutil.rotateY(movement)

	local handAngle = sine * 0.05
	local handMoveY = cosineSlow * 3.0
	local handMoveX = math.abs(cosineSlow * 4.0)
	local righthand = chr.handBone(skeleton, "righthand", skeletonAttr)
	righthand.translation = vec3.new(skeletonAttr.handRight + 0.1 + handMoveX, handMoveY, skeletonAttr.handForward)
	righthand.orientation = boneutil.rotateX(handAngle);

	local lefthand = chr.handBone(skeleton, "lefthand", skeletonAttr)
	lefthand = boneutil.boneMirrorXZ(righthand)
	lefthand.orientation = boneutil.rotateX(-handAngle)

	local footAngle = cosine * 0.5
	local footMoveY = cosine * 0.001
	local rightfoot = chr.footBone(skeleton, "rightfoot", skeletonAttr)
	rightfoot.translation = vec3.new(skeletonAttr.footRight, skeletonAttr.hipOffset - footMoveY, 0.0)
	rightfoot.orientation = boneutil.rotateX(footAngle)

	local leftfoot = skeleton:bone("leftfoot")
	leftfoot = boneutil.mirrorX(rightfoot)
	leftfoot.orientation = boneutil.rotateX(-footAngle)

	local tool = skeleton:bone("tool")
	tool.scale = vec3.new(skeletonAttr.toolScale * 0.8)
	tool.translation = vec3.new(skeletonAttr.toolRight, skeletonAttr.pantsY, skeletonAttr.toolForward)
	tool.orientation = boneutil.rotateYZ(math.rad(-90.0), math.rad(110.0))

	local rightshoulder = chr.shoulderBone(skeleton, "rightshoulder", skeletonAttr, boneutil.rotateX(sine * 0.15))

	local torso = skeleton:torsoBone(skeletonAttr.scaler)
	torso.translation = vec3.new(0.0, 0.5 + sine * 0.04, -skeletonAttr.beltY * torso.scale.z)
	torso.orientation = boneutil.rotateXZ(c_halfPi + -0.2 + cosine * 0.15, cosine * 0.1)

	local glider = skeleton:bone("glider")
	glider = skeleton:zeroBone()
	local leftshoulder = skeleton:bone("leftshoulder")
	leftshoulder = boneutil.mirrorX(rightshoulder)
end