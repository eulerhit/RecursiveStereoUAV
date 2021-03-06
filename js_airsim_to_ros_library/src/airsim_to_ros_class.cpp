///
/// @file
/// @copyright Copyright (C) 2017, Jonathan Bryan Schmalhofer
///
/// @brief AirSimToRos Implementation
///

#include "js_airsim_to_ros_library/airsim_to_ros_class.h"

namespace js_airsim_to_ros_library
{

AirSimToRosClass::AirSimToRosClass(std::string const& addr)
    : zmq_context_(1)
    , zmq_subscriber_(zmq_context_, ZMQ_SUB)
{
    zmq_subscriber_.setsockopt(ZMQ_IDENTITY, "AirSimToRosSubscriber", 5);
    zmq_subscriber_.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    zmq_subscriber_.setsockopt(ZMQ_RCVTIMEO, 5000);
    zmq_subscriber_.connect(addr);
    
    // Header
    image_header_seq_           = 0;
    image_header_stamp_sec_     = 0;
    image_header_stamp_nsec_    = 0;
    image_header_frame_id_      = "";
    // Image
    image_height_               = 0;
    image_width_                = 0;
    image_encoding_             = "";
    image_is_bigendian_         = 0;
    image_step_                 = 0;
    image_data_                 = NULL;
    image_data_size_            = 0;
}
    
AirSimToRosClass::~AirSimToRosClass()
{
    free(image_data_);
}

int8_t AirSimToRosClass::ReceivedMessage()
{
    zmq::message_t zmq_received_message;
    zmq_subscriber_.recv(&zmq_received_message);

    flatbuffers::FlatBufferBuilder fbb;
    airsim_to_ros::ImageBuilder builder(fbb);

    // Verify the message contents are trustworthy.
    flatbuffers::Verifier verifier((const unsigned char *)zmq_received_message.data(),
                                                          zmq_received_message.size());
    int8_t return_value = 0;


    if (zmq_received_message.size() > 0)
    {
      // Make sure the message has valid data.
      if (airsim_to_ros::VerifyImageBuffer(verifier))
      {
          auto fb_image_rcvd = airsim_to_ros::GetImage(zmq_received_message.data());

          // Header
          image_header_seq_           = fb_image_rcvd->header()->seq();
          image_header_stamp_sec_     = fb_image_rcvd->header()->stamp()->sec();
          image_header_stamp_nsec_    = fb_image_rcvd->header()->stamp()->nsec();
          image_header_frame_id_      = fb_image_rcvd->header()->frame_id()->c_str();
          // Image
          image_height_               = fb_image_rcvd->height();
          image_width_                = fb_image_rcvd->width();
          image_encoding_             = fb_image_rcvd->encoding()->c_str();
          image_is_bigendian_         = fb_image_rcvd->is_bigendian();
          image_step_                 = fb_image_rcvd->step();
          image_data_                 = (std::uint8_t*)realloc(image_data_, zmq_received_message.size());
          image_data_size_            = image_step_ * image_height_;
          memcpy(image_data_, fb_image_rcvd->data(), image_data_size_);
          return_value                = 1;
      }
      else
      {
          return_value                = -1;
      }
    }

    return return_value;
}
    
uint32_t AirSimToRosClass::GetImageHeaderSeq()
{
    return image_header_seq_;
}
    
uint32_t AirSimToRosClass::GetImageHeaderStampSec()
{
    return image_header_stamp_sec_;
}

uint32_t AirSimToRosClass::GetImageHeaderStampNsec()
{
    return image_header_stamp_nsec_;
}

std::string AirSimToRosClass::GetImageHeaderFrameid()
{
    return image_header_frame_id_;
}

uint32_t AirSimToRosClass::GetImageHeight()
{
    return image_height_;
}

uint32_t AirSimToRosClass::GetImageWidth()
{
    return image_width_;
}

std::string AirSimToRosClass::GetImageEncoding()
{
    return image_encoding_;
}

uint8_t AirSimToRosClass::GetImageIsBigendian()
{
    return image_is_bigendian_;
}

uint32_t AirSimToRosClass::GetImageStep()
{
    return image_step_;
}

std::uint8_t* AirSimToRosClass::GetImageData()
{
    return image_data_;
}
    
size_t AirSimToRosClass::GetImageDataSize()
{
    return image_data_size_;
}
}  // namespace js_airsim_to_ros_library

