#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include "Object.hpp"

class Render
{
private:
    vector<Object> objects;
public:
    Render(vector<Object> objects) : objects(objects) {}
    ~Render(){
        objects.clear();
    }
    void draw(){
        for (auto o : objects) {
            o.draw();
        }
    }
};