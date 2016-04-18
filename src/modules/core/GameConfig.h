#pragma once

namespace cfg {

constexpr const char *ClientMouseRotationSpeed = "cl_cammouserotspeed";
constexpr const char *ClientMouseSpeed = "cl_cammousespeed";
// the max pitch should not be bigger than 89.9 - because at 90 we have a visual switch
constexpr const char *ClientCameraMaxPitch = "cl_cammaxpitch";
constexpr const char *ClientEmail = "cl_email";
constexpr const char *ClientPassword = "cl_password";
// name of the player that is visible by other players
constexpr const char *ClientName = "cl_name";
constexpr const char *ClientVSync = "cl_vsync";
// the port where the server is listening on that the client wants to connect to
constexpr const char *ClientPort = "cl_port";
// the host where the server is running on that the client wants to connect to
constexpr const char *ClientHost = "cl_host";
constexpr const char *ClientFullscreen = "cl_fullscreen";
constexpr const char *ClientMultiSampleSamples = "cl_multisamplesamples";
constexpr const char *ClientMultiSampleBuffers = "cl_multisamplebuffers";

constexpr const char *ServerAutoRegister = "sv_autoregister";
constexpr const char *ServerUserTimeout = "sv_usertimeout";
// the server side seed that is used to create the world
constexpr const char *ServerSeed = "sv_seed";
constexpr const char *ServerHost = "sv_host";
constexpr const char *ServerPort = "sv_port";
constexpr const char *ServerMaxClients = "sv_maxclients";

constexpr const char *CoreLogLevel = "core_loglevel";

// The size of the chunk that is extracted with each step
constexpr const char *VoxelChunkSize = "voxel_chunksize";

constexpr const char *DatabaseName = "db_name";
constexpr const char *DatabaseHost = "db_host";
constexpr const char *DatabasePassword = "db_pw";
constexpr const char *DatabaseUser = "db_user";

constexpr const char *AppHomePath = "app_homepath";
constexpr const char *AppBasePath = "app_basepath";

}
