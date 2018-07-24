#include "main_window.h"
#include "hellovulkantexture.h"

#include <QtGui/QVulkanInstance>
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QVulkanInstance inst;

  inst.setLayers(QByteArrayList()
                 << "VK_LAYER_GOOGLE_threading"
                 << "VK_LAYER_LUNARG_parameter_validation"
                 << "VK_LAYER_LUNARG_object_tracker"
                 << "VK_LAYER_LUNARG_core_validation"
                 << "VK_LAYER_LUNARG_image"
                 << "VK_LAYER_LUNARG_swapchain"
                 << "VK_LAYER_GOOGLE_unique_objects");


  if (!inst.create())
      qFatal("Failed to create Vulkan instance: %d", inst.errorCode());

  VulkanWindow *w = new VulkanWindow;
  w->setVulkanInstance(&inst);
  VulkanWindow *w2 = new VulkanWindow;
  w2->setVulkanInstance(&inst);

  MainWindow mainWindow(w, w2);

  mainWindow.resize(1280, 720);
  mainWindow.show();

  return app.exec();
}