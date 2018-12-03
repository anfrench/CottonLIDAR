/*
this is mosytly psudo-code for the moment to deturmin how i'd like to; int the future, merge a picture with a point cloud. 
*/


pointcloud::pointCloud<pointZYZ> cloud_xyz;
pointCloud::popintCloud<pointXYZRGB> cloud_xyzrgb;
unit32[width][length] immage;

//this would be a two step process
//below this step is done while also adding color
//copyPointCloud(cloud_xyz, cloud_xyzrgb);

cloud_xyzrgb.points.resize(cloud_xyz.size());
for (size_t i = 0; i < cloud_xyz.points.size(); i++) 
{
    cloud_xyzrgb.points[i].x = cloud_xyz.points[i].x;
    cloud_xyzrgb.points[i].y = cloud_xyz.points[i].y;
    cloud_xyzrgb.points[i].z = cloud_xyz.points[i].z;
    
    float cloudMaxx,cloudMaxy;
    float cloudminx, cloudminy;
    float range, domain;
    range = (cloudmaxy-cloudminy)*height;
    domain=(cloudmaxx-cloudminx) * width;
    

	cloud_xyzrgb.points[i].rgb= 
	immage
	[(int)(((cloud_xyzRgb.points[i].x-cloudminx))*domain)]
	[(int)(((cloud_xyzRgb.points[i].y-cloudminy))*range)];

}










