#include "pch.h"
#include "screenShot.h"

int screenShot() {
	vtkSmartPointer<vtkSphereSource> sphereSource = 
		vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetCenter(0.0, 0.0, 0.0);
	sphereSource->SetRadius(5.0);
	sphereSource->Update();

	// Visualize
	vtkSmartPointer<vtkPolyDataMapper> mapper = 
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(sphereSource->GetOutputPort());

	vtkSmartPointer<vtkActor> actor = 
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	vtkSmartPointer<vtkRenderer> renderer = 
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = 
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindow->SetAlphaBitPlanes(1); //enable usage of alpha channel

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = 
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	renderer->AddActor(actor);
	renderer->SetBackground(1,1,1); // Background color white

	renderWindow->Render();

	// Screenshot  
	vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = 
		vtkSmartPointer<vtkWindowToImageFilter>::New();
	windowToImageFilter->SetInput(renderWindow);
	windowToImageFilter->SetMagnification(3); //set the resolution of the output image (3 times the current resolution of vtk render window)
	windowToImageFilter->SetInputBufferTypeToRGBA(); //also record the alpha (transparency) channel
	windowToImageFilter->ReadFrontBufferOff(); // read from the back buffer
	windowToImageFilter->Update();

	vtkSmartPointer<vtkPNGWriter> writer = 
		vtkSmartPointer<vtkPNGWriter>::New();
	writer->SetFileName("screenshot2.png");
	writer->SetInputConnection(windowToImageFilter->GetOutputPort());
	writer->Write();

	renderWindow->Render();  
	renderer->ResetCamera();
	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}

