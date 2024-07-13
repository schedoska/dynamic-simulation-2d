#include "Scene.h"
#include <iostream>

Scene::Scene()
{
}

void Scene::addObject(Object* object)
{
	m_objects.push_back(object);
}

bool Scene::checkCollisionSAT(Object* objectA, Object* objectB)
{
	std::vector<Math::Vector> &AVertexList = objectA->getVertexList();
	std::vector<Math::Vector> &BVertexList = objectB->getVertexList();
	int AVertexCount = AVertexList.size();
	int BVertexCount = BVertexList.size();

    std::vector<Math::Vector> axesList;
    axesList.reserve(AVertexCount + BVertexCount);
    for (int i = 0; i < AVertexCount; i++)
    {
        Math::Vector edge = objectA->mapToWorld(AVertexList[i])
            - objectA->mapToWorld(AVertexList[(i + 1) % AVertexCount]);
        axesList.push_back(Math::Vector(edge.y, -edge.x).normal());
    }
    for (int i = 0; i < BVertexCount; i++)
    {
        Math::Vector edge = objectB->mapToWorld(BVertexList[i])
            - objectB->mapToWorld(BVertexList[(i + 1) % BVertexCount]);
        axesList.push_back(Math::Vector(edge.y, -edge.x).normal());
    }

    for (Math::Vector axis : axesList)
    {
        AxisProjectionData axisProjectionA = projectObjectOnAxis(objectA, axis);
        AxisProjectionData axisProjectionB = projectObjectOnAxis(objectB, axis);

        if (axisProjectionB.minVal > axisProjectionA.maxVal || 
            axisProjectionB.maxVal < axisProjectionA.minVal) return false;
    }
	return true;
}

void Scene::checkCollisions()
{
    for (Object* object : m_objects) object->setCollidingState(false);

    for (Object* objectA : m_objects)
    {
        for (Object* objectB : m_objects)
        {
            if (objectA == objectB) continue;
            if (checkCollisionSAT(objectA, objectB))
            {
                objectA->setCollidingState(true);
                objectB->setCollidingState(true);
            }
        }
    }

    if (m_bouncingArea == false) return;
    for (Object* object : m_objects)
    {
        Math::Vector pos = object->getPosition();
        if (pos.x < m_bouncingAreaRectangle.x || pos.x > m_bouncingAreaRectangle.x + m_bouncingAreaRectangle.width)
            object->setVelocity(Math::Vector(-object->getVelocity().x, object->getVelocity().y));
        if (pos.y < m_bouncingAreaRectangle.y || pos.y > m_bouncingAreaRectangle.y + m_bouncingAreaRectangle.height)
            object->setVelocity(Math::Vector(object->getVelocity().x, -object->getVelocity().y));
    }
}

void Scene::setBouncingArea(const BouncingRectangleArea bouncingArea)
{
    m_bouncingAreaRectangle = bouncingArea;
}

void Scene::enableBouncingArea()
{
    m_bouncingArea = true;
}

void Scene::disableBouncingArea()
{
    m_bouncingArea = false;
}

void Scene::update(float dt)
{
    for (Object* obj : m_objects) obj->update(dt);
}

void Scene::debugDraw(sf::RenderWindow& window)
{
    for (Object* obj : m_objects) obj->debugDraw(window);
}

AxisProjectionData Scene::projectObjectOnAxis(Object* object, const Math::Vector& axis)
{
    AxisProjectionData axisProjectionData;
    std::vector<Math::Vector>& vertexList = object->getVertexList();

    axisProjectionData.maxVal = -std::numeric_limits<float>::max();
    axisProjectionData.minVal = std::numeric_limits<float>::max();

    for (Math::Vector vertex : vertexList)
    {
        float v = object->mapToWorld(vertex).dotProduct(axis);
        if (v > axisProjectionData.maxVal) axisProjectionData.maxVal = v;
        if (v < axisProjectionData.minVal) axisProjectionData.minVal = v;
    }
    return axisProjectionData;
}

BouncingRectangleArea::BouncingRectangleArea(float _x, float _y, float _width, float _height)
    :x(_x), y(_y), width(_width), height(_height) {}

bool BouncingRectangleArea::contains(const Math::Vector& vector)
{
    if (vector.x > x + width || vector.x < x) return false;
    if (vector.y > y + height || vector.y < y) return false;
    return true;
}
