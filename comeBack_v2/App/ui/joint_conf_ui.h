#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include "../tools/joint_handler.h"

#include <functional>

class joint_conf
{
public:
	virtual void draw() {};
};

class joint_conf_ui
{
public:
	joint_conf_ui();
	void set_target(dble_joint* target);
	void draw();

private:
	dble_joint* _target;
	joint_conf* _jc;
};

class spring_joint_conf : public joint_conf
{
public:
	spring_joint_conf(ds2::spring_joint* target);
	void draw() override;

private:
	ds2::spring_joint* _target;
};

class hinge_joint_conf : public joint_conf
{
public:
	hinge_joint_conf(ds2::hinge_joint* target);
	void draw() override;

private:
	ds2::hinge_joint* _target;
};

class motor_joint_conf : public joint_conf
{
public:
	motor_joint_conf(ds2::motor_joint* target);
	void draw() override;

private:
	ds2::motor_joint* _target;
};

