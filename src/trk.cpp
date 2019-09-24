#include "pch.h"
#include "trk.h"

struct TVHeader {
	char id_string[6];
	short int dim[3];
	float voxel_size[3];
	float origin[3];
	short int n_scalars;
	char scalar_name[10][20];
	short int n_properties;
	char property_name[10][20];
	float vox_to_ras[4][4];
	char reserved[444];
	char voxel_order[4];
	char pad2[4];
	float image_orientation_patient[6];
	char pad1[2];
	unsigned char invert_x;
	unsigned char invert_y;
	unsigned char swap_xy;
	unsigned char swap_yz;
	unsigned char swap_sx;
	int n_count;
	int version;
	int hdr_size;
};

vtkSmartPointer<vtkPolyData> _readPolyData(const string& fileName);
int showTrk() {
	//vtkSmartPointer<vtkSphereSource> sphereSource = 
	//	vtkSmartPointer<vtkSphereSource>::New();
	//sphereSource->SetCenter(0.0, 0.0, 0.0);
	//sphereSource->SetRadius(5.0);
	//sphereSource->Update();

	//// Visualize
	//vtkSmartPointer<vtkPolyDataMapper> mapper2 = 
	//	vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper2->SetInputConnection(sphereSource->GetOutputPort());

	//vtkSmartPointer<vtkActor> actor2 = 
	//	vtkSmartPointer<vtkActor>::New();
	//actor2->SetMapper(mapper2);

	// Visualize
	vtkSmartPointer<vtkPolyDataMapper> mapper = 
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(_readPolyData("aaa.trk"));

	vtkSmartPointer<vtkActor> actor = 
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	//vtkSmartPointer<vtkTransform> trans =
	//	vtkSmartPointer<vtkTransform>::New();
	//trans->RotateX(180);
	//trans->RotateY(180);
	//actor->SetUserTransform(trans);

	vtkSmartPointer<vtkNamedColors> colors =
		vtkSmartPointer<vtkNamedColors>::New();
	actor->GetProperty()->SetColor(
			colors->GetColor4d("SpringGreen").GetData());

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
	//renderer->AddActor(actor2);
	renderer->SetBackground(0,0,0); // Background color white

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}
vtkSmartPointer<vtkPolyData> _readPolyData(const string& fileName) {
	vtkSmartPointer<vtkPolyData> pd = vtkSmartPointer<vtkPolyData>::New();
	TVHeader header;
	size_t trackNumPts = 3;
	size_t headSize = sizeof(TVHeader);

	fstream strm(fileName, ios::in | ios::binary);
	if(strm.fail()) {
		throw;
	}
	strm.seekg(0, ios::end);
	size_t fileSize = strm.tellg();
	if(fileSize < headSize) {
		throw;
	}

	strm.seekg(0);
	strm.read((char*)&header, sizeof(TVHeader));

	strm.seekg(headSize, ios::beg);
	if (header.n_scalars == 0 && header.n_properties == 0) {
		vtkNew<vtkCellArray>  lines;
		vtkNew<vtkFloatArray> pointdata;
		pointdata->SetNumberOfComponents(trackNumPts);
		size_t cur_idx = 0; // global index to point list
		int    cur_pts = 0; // point count: int by TrackVis definition
		while ((size_t)strm.tellg() < fileSize) {
			strm.read((char*)&cur_pts, sizeof(int));
			char* points = (char*)pointdata->WriteVoidPointer(
					cur_idx * trackNumPts,
					cur_pts * trackNumPts);
			strm.read(// get pointer for direct write to new points
					points,
					// points * components * size
					cur_pts * trackNumPts * sizeof(float));

			/* associate point indices to new cell for this line */
			std::vector<vtkIdType> idlist(cur_pts);
			std::iota(idlist.begin(), idlist.end(), cur_idx);
			lines->InsertNextCell(cur_pts, idlist.data());

			cur_idx += cur_pts;
		}
		vtkNew<vtkPoints> points;
		points->SetDataTypeToFloat();
		points->SetData(pointdata.GetPointer());
		pd->SetLines(lines.GetPointer());
		pd->SetPoints(points.GetPointer());
	} else {
		throw;
	}
	return pd;
}

