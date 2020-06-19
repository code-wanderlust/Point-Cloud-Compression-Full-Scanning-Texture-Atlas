#ifndef VOXEL_SLICE_SCANNED_DATA
#define VOXEL_SLICE_SCANNED_DATA

#include <vector>


class Voxel_Slice_Scanned_Data
{
public:
	Voxel_Slice_Scanned_Data();
	Voxel_Slice_Scanned_Data(const Voxel_Slice_Scanned_Data &T);
	~Voxel_Slice_Scanned_Data();


	std::vector<std::vector<int>> voxel_idx_sequences; //peeling_chain_codes;
	/****************************************************/
	std::vector<int> blob_index_for_before_slice;   //CKvXvectorInt &io_EQUIVALENCE_list;
	std::vector<int> list_of_index_for_connectivity;//CKvXvectorInt &out_list_of_object_labels_for_SECOND_slice;
	/****************************************************/
	std::vector<int> list_of_label_for_voxel_idx_sequences;


	void cp_Copy(Voxel_Slice_Scanned_Data &T)
	{
		this->voxel_idx_sequences			 = T.voxel_idx_sequences;
		this->blob_index_for_before_slice    = T.blob_index_for_before_slice;
		this->list_of_index_for_connectivity = T.list_of_index_for_connectivity;
	}

};

#endif