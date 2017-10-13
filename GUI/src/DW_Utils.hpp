#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "opencv2/opencv.hpp"

using namespace std;;

namespace DW_Utils {

	class associator {
		ifstream ifs;
		std::vector<vector<std::string>> CD_association;
		int idx = 0;
		string dataset_path;

	public:
		associator(string _dataset_path) {

			dataset_path = _dataset_path;
			string association_file = dataset_path + "/associations.txt";
			ifs.open(association_file, std::ios::in);

			while (!ifs.eof())
			{
				string color_stamp,depth_stamp;
				ifs >> color_stamp >> color_stamp >> depth_stamp >> depth_stamp;
				cout << color_stamp << " " << depth_stamp << endl;
				vector<string> asso;
				asso.push_back(dataset_path+color_stamp);
				asso.push_back(dataset_path+depth_stamp);
				asso.push_back(dataset_path+color_stamp.replace(0,3,"semantics"));
				CD_association.push_back(asso);
			}
		}
		vector<string> next(){
			return CD_association[idx++];
		}
	};
//	associator asso("/media/dongwonshin/Ubuntu Data/Datasets/ICL-NUIM/living_room_traj0_frei_png/");
	associator asso("/media/dongwonshin/Ubuntu Data/Datasets/TUM/3D Object Reconstruction/rgbd_dataset_freiburg3_teddy/rgbd_dataset_freiburg3_teddy/");



	cv::Mat *rgb_img;
	cv::Mat *depth_img;
	cv::Mat *semantic_img;

	unsigned char * semantics;


	void TUMDatasetLoad() {

		if (rgb_img != NULL){
			delete(rgb_img);
		}
		if (depth_img != NULL){
			delete(depth_img);
		}
		if (semantic_img != NULL){
			delete(semantic_img);
		}

		rgb_img = new cv::Mat();
		depth_img = new cv::Mat();
		semantic_img = new cv::Mat();

		vector<string> cur_asso = asso.next();
//		cout << cur_asso[0] << endl << cur_asso[1] << endl;

		*rgb_img = cv::imread(cur_asso[0]);
        cv::cvtColor(*rgb_img, *rgb_img, cv::COLOR_BGR2RGB);

		*depth_img = cv::imread(cur_asso[1], CV_LOAD_IMAGE_ANYDEPTH);
		for (int i = 0 ; i < 640 ; i++)
		for (int j = 0 ; j < 480 ; j++)
		{
			// Dong-Won Shin: magic factor.... I don't know why.
			depth_img->at<unsigned short>(j,i) /= 10;
		}

		*semantic_img = cv::imread(cur_asso[2]);
		cv::cvtColor(*semantic_img, *semantic_img, cv::COLOR_BGR2RGB);

//		{
//			cv::imshow("rgb_img", rgb_img);
//			cv::imshow("depth_img", depth_img);
//			cv::imshow("semantic_img", semantic_img);
//			cv::waitKey();
//		}
	}

	void ICLNUIMDatasetLoad() {

			if (rgb_img != NULL){
				delete(rgb_img);
			}
			if (depth_img != NULL){
				delete(depth_img);
			}

			rgb_img = new cv::Mat();
			depth_img = new cv::Mat();

			vector<string> cur_asso = asso.next();
//			cout << cur_asso[0] << endl << cur_asso[1] << endl;

			*rgb_img = cv::imread(cur_asso[1]);
	        cv::cvtColor(*rgb_img, *rgb_img, cv::COLOR_BGR2RGB);

			*depth_img = cv::imread(cur_asso[0], CV_LOAD_IMAGE_ANYDEPTH);
			for (int i = 0 ; i < 640 ; i++)
			for (int j = 0 ; j < 480 ; j++)
			{
				// Dong-Won Shin: magic factor.... I don't know why.
				depth_img->at<unsigned short>(j,i) /= 10;
			}

	//		{
	//			cv::imshow("rgb_img", rgb_img);
	//			cv::imshow("depth_img", depth_img);
	//			cv::waitKey();
	//		}
		}
}
