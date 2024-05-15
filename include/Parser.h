#pragma once

class Subrecord
{
public:
	Subrecord() : subrecordType(4, '\0') {}

	void parse(std::istream& stream);

private:
	std::string subrecordType;
	std::shared_ptr<char[]> data; // empty if skipped
};

class Record
{
public:
	Record() : recordType(4, '\0') {}

	void parse(std::istream& stream);

	const RE::FormID getFormID() const { return formID; }

	const std::vector<Subrecord>& getSubrecords() const { return subrecords; }

private:
	std::uint32_t readHeader(std::istream& stream);
	void parseSubrecord(std::istream& stream, std::uint32_t size);

	bool IsCompressed() const { return (flags & 0x00040000) != 0; };

	std::string recordType;
	std::uint32_t flags; // empty if skipped
	RE::FormID formID; // empty if skipped
	std::vector<Subrecord> subrecords; // empty if skipped

	bool skipped = false;
};

class Group
{
public:
	void parse(std::istream& stream);

	const std::vector<Record>& getRecords() const { return records; }
private:
	std::uint32_t readHeader(std::istream& stream);
	void parseRecordsAndSubgroups(std::istream& stream, std::uint32_t size);
	std::uint32_t nextType(std::istream& stream) const;

	std::vector<Record> records;
};

class Parser : public ISingleton<Parser>
{
public:
	void load(const std::filesystem::path& path);

	Record fileheader;
};