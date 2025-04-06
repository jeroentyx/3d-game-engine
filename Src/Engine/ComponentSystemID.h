#pragma once

enum class COMPONENTNAME : unsigned char
{
	Comp_Physics_Component,
	Comp_Transform_Component,
	Comp_Mesh_Component,
	Comp_Light_Component,
	Comp_Script_Component,
	Comp_Static_Mesh_Component,
	Comp_Animated_Mesh_Component,
	Comp_Camera_Component,
	Comp_Audio_Component,
	Comp_UI_Component,
	Comp_GUI_Component,
	Comp_UI_Sprite2D_Component,
	Comp_Skybox_Component,
	Comp_Particle_Component,
	Comp_Smoke_Component,
	Comp_Font_Component,
	Comp_BillBoard_Component,
	Comp_None
};

enum class SYSNAME : unsigned char
{
	NO_SYSTEM,
	SYS_SERIALIZER,
	SYS_GRAPHICS,
	SYS_PHYSICS,
	SYS_LOGIC,
	SYS_AUDIO,
	SYS_UI,
	SYS_SANDBOX
};

