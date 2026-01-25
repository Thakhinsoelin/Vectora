#include "Vectora.h"

class SandBox : public Vectora::Application {
public:
    SandBox() {

    }
    ~SandBox() {

    }
};

Vectora::Application* Vectora::CreateApplication() {
    return new SandBox();
}
