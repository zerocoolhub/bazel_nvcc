// Simple C++ program to display "Hello World"
 
// Header file for input output functions
#include <QtGui/QGuiApplication>
#include <QtGui/qvulkaninstance.h>
#include <QtGui/QWindow>
#include <QtCore/QLoggingCategory>

#include "hellovulkanwindow.h"
 
using namespace std;
 
// main function -
// where the execution of program begins
int main(int argc, char** argv)
{
	QGuiApplication app(argc, argv);

	QLoggingCategory::setFilterRules(QStringLiteral("qt.vulkan=true"));
  QVulkanInstance inst;
    

  printf("here");
     inst.setLayers(QByteArrayList()
                   << "VK_LAYER_GOOGLE_threading"
                   << "VK_LAYER_LUNARG_parameter_validation"
                   << "VK_LAYER_LUNARG_object_tracker"
                   << "VK_LAYER_LUNARG_core_validation"
                   << "VK_LAYER_LUNARG_image"
                   << "VK_LAYER_LUNARG_swapchain"
                   << "VK_LAYER_GOOGLE_unique_objects");

    printf("here");
    if (!inst.create())
        qFatal("Failed to create Vulkan instance: %d", inst.errorCode());


  	VulkanWindow w;
    w.setVulkanInstance(&inst);

    w.resize(1024, 768);
    w.show();
    return app.exec();
}