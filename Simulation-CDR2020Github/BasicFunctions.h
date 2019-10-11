#pragma once
#include"stdafx.h"

#include"Point.h"
#include"Constantes.h"

float radian(float angle);

float degre(float angle);

float val_abs(float a);

float part_pos(float a);

float dist(float a, float b);

float dist(Point p);

float properAngleRad(float ang);

int sgn(float a);

float xconv(int x);
float yconv(int y);
float aconv(int angle);

float xconv2(int x);

float CmToPx(float dist);

Point operator+(Point const& a, Point const& b);

Point operator-(Point const& a, Point const& b);

Point operator*(float const t, Point const& a);

Point operator*(Point const& a, float const t);

bool operator!=(Point const& a, Point const& b);

