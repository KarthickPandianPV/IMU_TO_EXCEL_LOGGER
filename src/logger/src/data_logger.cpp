#include "ros/ros.h"
#include "geometry_msgs/Vector3.h"
#include "xlsxwriter.h"
#include <time.h>

ros::Subscriber orientation_sub;
ros::Subscriber sub1;
ros::Subscriber sub2;
ros::Subscriber sub3;

geometry_msgs::Vector3 euler_orientation;
geometry_msgs::Vector3 linear_acceleration;
geometry_msgs::Vector3 angular_velocity;
geometry_msgs::Vector3 magnetic_field;

bool acc_data_available = false, gyro_data_available = false, mag_data_available = false, orien_data_available = false;

time_t rawtime;
struct tm *timeinfo;
char date_string[20];
char time_string[20];

lxw_workbook  *workbook  = workbook_new("imu_data.xlsx");
lxw_format *format_bold = workbook_add_format(workbook);
lxw_format *format_txt_clr = workbook_add_format(workbook);

void orientationCallBack(const geometry_msgs::Vector3& orientation_msg);
void accelerometerCallBack(const geometry_msgs::Vector3& acceleration_msg);
void gyroscopeCallBack(const geometry_msgs::Vector3& angular_velocity_msg);
void magnetometerCallBack(const geometry_msgs::Vector3& magnetic_field_msg);

float roll,pitch,yaw;
float ax,ay,az,gx,gy,gz,mx,my,mz;

int main(int argc,char** argv) 
{
    int row=1;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(date_string, sizeof(date_string), "%Y-%m-%d", timeinfo);
    strftime(time_string, sizeof(time_string), "%H-%M-%S", timeinfo);

    lxw_worksheet *worksheet = workbook_add_worksheet(workbook,time_string);
    format_set_bold(format_bold);
    format_set_font_color(format_txt_clr,LXW_COLOR_BLUE);

    worksheet_write_string(worksheet, 0, 0, "Timestamp",format_bold);
    worksheet_write_string(worksheet, 0, 1, "ax", format_bold);
    worksheet_write_string(worksheet, 0, 2, "ay", format_bold);
    worksheet_write_string(worksheet, 0, 3, "az", format_bold);
    worksheet_write_string(worksheet, 0, 5, "gx", format_bold);
    worksheet_write_string(worksheet, 0, 6, "gy", format_bold);
    worksheet_write_string(worksheet, 0, 7, "gz", format_bold);
    worksheet_write_string(worksheet, 0, 9, "mx", format_bold);
    worksheet_write_string(worksheet, 0, 10, "my", format_bold);
    worksheet_write_string(worksheet, 0, 11, "mz", format_bold);
    worksheet_write_string(worksheet, 0, 13, "roll", format_bold);
    worksheet_write_string(worksheet, 0, 14, "pitch", format_bold);
    worksheet_write_string(worksheet, 0, 15, "yaw", format_bold);

    ros::init(argc, argv, "data_logger");
    ros::NodeHandle n;
    orientation_sub=n.subscribe("euler_orientation", 1, orientationCallBack);
    sub1=n.subscribe("linear_acceleration", 1, accelerometerCallBack);
    sub2=n.subscribe("angular_velocity", 1, gyroscopeCallBack);
    sub3=n.subscribe("magnetic_field", 1, magnetometerCallBack);
    
    while (ros::ok())
       {
           if((acc_data_available && gyro_data_available) && (mag_data_available && orien_data_available))
           {
              time(&rawtime);
              timeinfo = localtime(&rawtime);
              strftime(time_string, sizeof(time_string), "%H-%M-%S", timeinfo);
              
              worksheet_write_string(worksheet, row, 0, time_string,format_txt_clr);
              worksheet_write_number(worksheet, row, 1, ax, NULL);
              worksheet_write_number(worksheet, row, 2, ay, NULL);
              worksheet_write_number(worksheet, row, 3, az, NULL);
              worksheet_write_number(worksheet, row, 5, gx, NULL);
              worksheet_write_number(worksheet, row, 6, gy, NULL);
              worksheet_write_number(worksheet, row, 7, gz, NULL);
              worksheet_write_number(worksheet, row, 9, mx, NULL);
              worksheet_write_number(worksheet, row, 10, my, NULL);
              worksheet_write_number(worksheet, row, 11, mz, NULL);
              worksheet_write_number(worksheet, row, 13, roll, format_txt_clr);
              worksheet_write_number(worksheet, row, 14, pitch, format_txt_clr);
              worksheet_write_number(worksheet, row, 15, yaw, format_txt_clr);
              row++;
              acc_data_available = false; gyro_data_available = false; mag_data_available = false,orien_data_available=false;
              std::cout<<orien_data_available<<acc_data_available<<gyro_data_available<<mag_data_available<<'\n';
           }
              ros::spinOnce();
       }

    
    return workbook_close(workbook);
}

void orientationCallBack(const geometry_msgs::Vector3& orientation_msg)
{
    roll = orientation_msg.x;
    pitch = orientation_msg.y;
    yaw = orientation_msg.z;

    orien_data_available = true;
}

void accelerometerCallBack(const geometry_msgs::Vector3& acceleration_msg)
{
    ax = acceleration_msg.x;
    ay = acceleration_msg.y;
    az = acceleration_msg.z;

    acc_data_available = true;
}

void gyroscopeCallBack(const geometry_msgs::Vector3& angular_velocity_msg)
{
    gx = angular_velocity_msg.x;
    gy = angular_velocity_msg.y;
    gz = angular_velocity_msg.z;
    
    gyro_data_available = true;
}

void magnetometerCallBack(const geometry_msgs::Vector3& magnetic_field_msg)
{
    mx = magnetic_field_msg.x;
    my = magnetic_field_msg.y;
    mz = magnetic_field_msg.z;

    mag_data_available = true;
}


