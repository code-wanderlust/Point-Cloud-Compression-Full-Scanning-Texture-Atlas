#include "stdafx.h"
#include "Voxel_Slice_Scanned_Data.h"


//*************************************************************************
Voxel_Slice_Scanned_Data::Voxel_Slice_Scanned_Data()
//*************************************************************************
{

}
//*************************************************************************
Voxel_Slice_Scanned_Data::Voxel_Slice_Scanned_Data(
	const Voxel_Slice_Scanned_Data &T)
//*************************************************************************
{
	this->voxel_idx_sequences = T.voxel_idx_sequences;
	this->blob_index_for_before_slice = T.blob_index_for_before_slice;
	this->list_of_index_for_connectivity = T.list_of_index_for_connectivity;
}
//*************************************************************************
Voxel_Slice_Scanned_Data::~Voxel_Slice_Scanned_Data()
//*************************************************************************
{

}
//*************************************************************************
