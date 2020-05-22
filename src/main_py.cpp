//#include "ObjLoader.h"
//#include "Render.h"
//#include "Rectangle.h"
//#include <chrono>
//#include <eigen3/Eigen/Geometry>
//#include <memory>
#include <pybind11/pybind11.h>

#include "SceneManager.h"
#define tracerr_MAJOR_VERSION 1
#define tracerr_MINOR_VERSION 0

// Coordinate frame of reference
// Camera is at (0,0,0)
// X positive to right
// Y positive up
// Z negative means far away
// Z positive is behind projection plane

float Material::ambient_light{0.05};

void RenderScene(char* text_proto){
        SceneManager sm;
        sm.LoadScene(text_proto);
}
void GenerateExample(char* text_proto){
        SceneManager sm;
        sm.SaveExampleScene(text_proto);
}
void GenerateTemplate(char* text_proto){
        SceneManager sm;
        sm.LoadScene(text_proto);
}
void GenerateTemplateNoComment(char* text_proto){
        SceneManager sm;
        sm.LoadScene(text_proto);
}
PYBIND11_MODULE(tracerr, m) {
        m.doc() = R"pbdoc(
        Pybind11 tracerr plugin
        -----------------------
        .. currentmodule:: tracerr
        .. autosummary::
           :toctree: _generate
           RenderScene
           GenerateExample
           GenerateTemplate
           GenerateTemplateNoComment
    )pbdoc";
        m.def("RenderScene", &RenderScene, R"pbdoc(
        Render scene described in text_proto file.
    )pbdoc");
        m.def("GenerateExample", &GenerateExample, R"pbdoc(
        Generates a textproto file of a Example scene and saves it to the specified file.
    )pbdoc");
        m.def("GenerateTemplate", &GenerateTemplate, R"pbdoc(
        Generates an empty Template with comments for a scene.
    )pbdoc");
        m.def("GenerateTemplateNoComment", &GenerateTemplateNoComment, R"pbdoc(
        Generates an empty Template WITHOUT comments for a scene.
    )pbdoc");
#ifdef VERSION_INFO
        m.attr("__version__") = VERSION_INFO;
#else
        m.attr("__version__") = "dev";
#endif
}


int main(int argc, char **argv) {

        return 0;
}
