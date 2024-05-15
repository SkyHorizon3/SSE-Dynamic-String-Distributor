#include "Parser.h"
#include <zlib.h>

// subrecords
void Subrecord::parse(std::istream& stream)
{
	stream.read(&subrecordType[0], 4);

	if (subrecordType != "DNAM"sv || subrecordType != "TRDT"sv)
		return;

	SKSE::log::info("SubrecordType: {}", subrecordType);

	std::uint16_t dataLength;
	stream.read(reinterpret_cast<char*>(&dataLength), sizeof(dataLength));

	data = std::make_shared<char[]>(dataLength);
	stream.read(data.get(), dataLength);
}

// records
void Record::parse(std::istream& stream)
{
	parseSubrecord(stream, readHeader(stream));
}

void Record::parseSubrecord(std::istream& stream, std::uint32_t size)
{
	if (skipped)
	{
		stream.seekg(size, std::ios::cur);
		skipped = false;
		return;
	}

	if (IsCompressed()) // skip for now
	{
		stream.seekg(size, std::ios::cur);
		SKSE::log::info("Parsed compressed record");

		//uncompress();


		return;
	}

	std::streampos endPos = stream.tellg() + static_cast<std::streamoff>(size);

	while (stream.good() && stream.tellg() < endPos)
	{
		Subrecord subrecord;
		subrecord.parse(stream);
		subrecords.emplace_back(subrecord);
	}

}

std::uint32_t Record::readHeader(std::istream& stream)
{
	stream.read(&recordType[0], 4);

	std::uint32_t subrecordSize;
	stream.read(reinterpret_cast<char*>(&subrecordSize), sizeof(subrecordSize));

	if (recordType != "INFO"sv)
	{
		stream.seekg(16, std::ios::cur);
		skipped = true;
		return subrecordSize;
	}

	SKSE::log::info("RecordType: {}", recordType);

	// read flags
	stream.read(reinterpret_cast<char*>(&flags), sizeof(flags));

	// read formID
	stream.read(reinterpret_cast<char*>(&formID), sizeof(formID));

	SKSE::log::info("FormID: {0:08X}", formID);

	// skip header length
	stream.seekg(8, std::ios::cur);

	return subrecordSize;
}

// Group
void Group::parse(std::istream& stream)
{
	parseRecordsAndSubgroups(stream, readHeader(stream));
}

void Group::parseRecordsAndSubgroups(std::istream& stream, std::uint32_t size)
{
	std::streampos endPos = stream.tellg() + static_cast<std::streamoff>(size);

	while (stream.good() && stream.tellg() < endPos)
	{
		if (nextType(stream) == 0x50555247) //GRUP
		{
			Group subGroup;
			subGroup.parse(stream);
			std::vector<Record> subGroupFormIds = subGroup.getRecords();
			this->records.insert(this->records.begin(), subGroupFormIds.begin(), subGroupFormIds.end());
		}
		else
		{
			Record record;
			record.parse(stream);

			if (record.getFormID() != 0xCCCCCCCC) // because of the skip
				records.emplace_back(record);
		}
	}

}

std::uint32_t Group::nextType(std::istream& stream) const
{
	uint32_t type;
	auto currentPos = stream.tellg();
	stream.read(reinterpret_cast<char*>(&type), sizeof(type));
	stream.seekg(currentPos); // Reset stream position to original

	return type;
}

std::uint32_t Group::readHeader(std::istream& stream)
{
	std::uint32_t groupSize;

	// Skip the group type and read the total group size
	stream.seekg(4, std::ios::cur);
	stream.read(reinterpret_cast<char*>(&groupSize), sizeof(groupSize));

	// Skip the rest of the header.
	stream.seekg(16, std::ios::cur);

	return groupSize - 24; // 4 (group type) + 4 (group size) + 16 (header size) = 24
}

// Main parser
void Parser::load(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
	{
		SKSE::log::error("Parse error: {}", path.filename().string());
		return;
	}

	std::uintmax_t fileSize = std::filesystem::file_size(path);

	std::ifstream inputFile(path, std::ios::binary);
	if (!inputFile)
	{
		SKSE::log::error("Failed to open file: {}", path.filename().string());
		return;
	}

	fileheader.parse(inputFile);

	while (inputFile.good() && (std::uintmax_t)inputFile.tellg() < fileSize)
	{
		Group group;
		group.parse(inputFile);


		for (const auto& record : group.getRecords())
		{
			SKSE::log::info("{0:08X}", record.getFormID());

		}

	}

}