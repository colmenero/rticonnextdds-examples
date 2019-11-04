/*****************************************************************************/
/*         (c) Copyright, Real-Time Innovations, All rights reserved.        */
/*                                                                           */
/*         Permission to modify and use for internal purposes granted.       */
/* This software is provided "as is", without warranty, express or implied.  */
/*                                                                           */
/*****************************************************************************/

#include "FileStreamWriter.hpp"
#include <rti/topic/to_string.hpp>
#include <rti/core/Exception.hpp>

using namespace rti::routing;
using namespace rti::routing::adapter;
using namespace rti::community::examples;

const std::string FileStreamWriter::OUTPUT_FILE_PROPERTY_NAME = "example.adapter.output_file";

FileStreamWriter::FileStreamWriter(const PropertySet &properties)
{
    std::string output_file_name;
    for (const auto &property : properties) {
        if (property.first == OUTPUT_FILE_PROPERTY_NAME) {
            output_file_name = property.second;
            output_file_.open(output_file_name);
            break;
        }
    }

    if (!output_file_.is_open()) {
        throw dds::core::IllegalOperationError(
                "Error opening output file: " + output_file_name);
    } else {
        std::cout << "Output file name: " << output_file_name << std::endl;
    }
};

int FileStreamWriter::write(
        const std::vector<dds::core::xtypes::DynamicData *> &samples,
        const std::vector<dds::sub::SampleInfo *> &infos)
{
    for (auto sample : samples) {
        std::string str = rti::topic::to_string(
                *sample, 
                rti::topic::PrintFormatProperty::Default());
        std::cout << "Received Sample: " << std::endl << str << std::endl;

        output_file_ << sample->value<std::string>("color") << "," 
                << sample->value<int32_t>("x") << "," 
                << sample->value<int32_t>("y") << "," 
                << sample->value<int32_t>("shapesize") << std::endl;
    }
    return 0;
}

FileStreamWriter::~FileStreamWriter()
{
    output_file_.close();
};
