// E-Waste Management & Recycling System
// Single-file C++17 version

#include <algorithm>
#include <cctype>
#include <cmath>
#include <ctime>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <regex>
#include <set>
#include <sstream>
#include <string>
#include <vector>


// ===== EWasteException.h =====


class EWasteException : public std::exception {
private:
    std::string message_;

public:
    explicit EWasteException(const std::string& message);
    const char* what() const noexcept override;
};

class InvalidWeight : public EWasteException {
public:
    explicit InvalidWeight(const std::string& message);
};

class InvalidEWasteType : public EWasteException {
public:
    explicit InvalidEWasteType(const std::string& message);
};

class InvalidID : public EWasteException {
public:
    explicit InvalidID(const std::string& message);
};

class InvalidInput : public EWasteException {
public:
    explicit InvalidInput(const std::string& message);
};

class ItemNotFound : public EWasteException {
public:
    explicit ItemNotFound(const std::string& message);
};

// ===== EWasteException.cpp =====

EWasteException::EWasteException(const std::string& message) : message_(message) {}

const char* EWasteException::what() const noexcept {
    return message_.c_str();
}

InvalidWeight::InvalidWeight(const std::string& message) : EWasteException(message) {}
InvalidEWasteType::InvalidEWasteType(const std::string& message) : EWasteException(message) {}
InvalidID::InvalidID(const std::string& message) : EWasteException(message) {}
InvalidInput::InvalidInput(const std::string& message) : EWasteException(message) {}
ItemNotFound::ItemNotFound(const std::string& message) : EWasteException(message) {}

// ===== EWasteItem.h =====


class EWasteItem {
public:
    enum class Condition {
        Working = 1,
        PartiallyWorking = 2,
        NonWorking = 3
    };

    enum class CollectionStatus {
        Registered = 1,
        Scheduled = 2,
        Collected = 3,
        Sorted = 4,
        Recycled = 5
    };

    EWasteItem(const std::string& id,
               const std::string& ownerName,
               const std::string& itemName,
               Condition condition,
               double weight,
               CollectionStatus status = CollectionStatus::Registered,
               const std::string& collectionDate = "",
               const std::string& collectionAddress = "");
    virtual ~EWasteItem();

    virtual std::string getType() const = 0;
    virtual std::string getCategory() const = 0;
    virtual int calculateReward() const = 0;
    virtual std::string getRecommendedAction() const = 0;
    virtual double getRewardRate() const = 0;
    virtual bool isHazardous() const;
    virtual std::string getSafetyWarning() const;

    const std::string& getId() const;
    const std::string& getOwnerName() const;
    const std::string& getItemName() const;
    Condition getCondition() const;
    double getWeight() const;
    CollectionStatus getStatus() const;
    const std::string& getCollectionDate() const;
    const std::string& getCollectionAddress() const;

    void setCondition(Condition condition);
    void setStatus(CollectionStatus status);
    void setCollectionInfo(const std::string& date, const std::string& address);

    // A larger reward means the item is more valuable for the sustainability program.
    bool operator>(const EWasteItem& other) const;

    static void validateId(const std::string& id);
    static std::string conditionToString(Condition condition);
    static std::string statusToString(CollectionStatus status);
    static Condition conditionFromInt(int value);
    static CollectionStatus statusFromInt(int value);
    static double conditionMultiplier(Condition condition);

protected:
    int calculateByRate(double typeRate) const;

private:
    std::string id_;
    std::string ownerName_;
    std::string itemName_;
    Condition condition_;
    double weight_;
    CollectionStatus status_;
    std::string collectionDate_;
    std::string collectionAddress_;
};

// ===== EWasteItem.cpp =====


EWasteItem::EWasteItem(const std::string& id,
                       const std::string& ownerName,
                       const std::string& itemName,
                       Condition condition,
                       double weight,
                       CollectionStatus status,
                       const std::string& collectionDate,
                       const std::string& collectionAddress)
    : id_(id),
      ownerName_(ownerName),
      itemName_(itemName),
      condition_(condition),
      weight_(weight),
      status_(status),
      collectionDate_(collectionDate),
      collectionAddress_(collectionAddress) {
    validateId(id_);
    if (ownerName_.empty() || itemName_.empty()) {
        throw InvalidInput("Owner name and item name cannot be empty.");
    }
    if (!std::isfinite(weight_) || weight_ <= 0.0 || weight_ > 1000.0) {
        throw InvalidWeight("Weight must be a finite value greater than 0 kg and no more than 1000 kg.");
    }
    if (condition_ < Condition::Working || condition_ > Condition::NonWorking) {
        throw InvalidInput("Invalid condition value.");
    }
    if (status_ < CollectionStatus::Registered || status_ > CollectionStatus::Recycled) {
        throw InvalidInput("Invalid collection status value.");
    }
}

EWasteItem::~EWasteItem() = default;

bool EWasteItem::isHazardous() const {
    return false;
}

std::string EWasteItem::getSafetyWarning() const {
    return "Follow normal e-waste handling and keep the item dry before collection.";
}

const std::string& EWasteItem::getId() const {
    return id_;
}

const std::string& EWasteItem::getOwnerName() const {
    return ownerName_;
}

const std::string& EWasteItem::getItemName() const {
    return itemName_;
}

EWasteItem::Condition EWasteItem::getCondition() const {
    return condition_;
}

double EWasteItem::getWeight() const {
    return weight_;
}

EWasteItem::CollectionStatus EWasteItem::getStatus() const {
    return status_;
}

const std::string& EWasteItem::getCollectionDate() const {
    return collectionDate_;
}

const std::string& EWasteItem::getCollectionAddress() const {
    return collectionAddress_;
}

void EWasteItem::setCondition(Condition condition) {
    if (condition < Condition::Working || condition > Condition::NonWorking) {
        throw InvalidInput("Invalid condition value.");
    }
    condition_ = condition;
}

void EWasteItem::setStatus(CollectionStatus status) {
    if (status < CollectionStatus::Registered || status > CollectionStatus::Recycled) {
        throw InvalidInput("Invalid collection status value.");
    }
    status_ = status;
}

void EWasteItem::setCollectionInfo(const std::string& date, const std::string& address) {
    if (date.empty() || address.empty()) {
        throw InvalidInput("Collection date and address cannot be empty.");
    }
    collectionDate_ = date;
    collectionAddress_ = address;
}

bool EWasteItem::operator>(const EWasteItem& other) const {
    return calculateReward() > other.calculateReward();
}

void EWasteItem::validateId(const std::string& id) {
    static const std::regex idPattern("EW[0-9]{4,}");
    if (!std::regex_match(id, idPattern)) {
        throw InvalidID("Invalid ID. Expected format such as EW1001.");
    }
}

std::string EWasteItem::conditionToString(Condition condition) {
    switch (condition) {
        case Condition::Working: return "Working";
        case Condition::PartiallyWorking: return "Partially Working";
        case Condition::NonWorking: return "Non-Working";
    }
    return "Unknown";
}

std::string EWasteItem::statusToString(CollectionStatus status) {
    switch (status) {
        case CollectionStatus::Registered: return "Registered";
        case CollectionStatus::Scheduled: return "Scheduled";
        case CollectionStatus::Collected: return "Collected";
        case CollectionStatus::Sorted: return "Sorted";
        case CollectionStatus::Recycled: return "Recycled";
    }
    return "Unknown";
}

EWasteItem::Condition EWasteItem::conditionFromInt(int value) {
    if (value < 1 || value > 3) {
        throw InvalidInput("Condition must be 1, 2, or 3.");
    }
    return static_cast<Condition>(value);
}

EWasteItem::CollectionStatus EWasteItem::statusFromInt(int value) {
    if (value < 1 || value > 5) {
        throw InvalidInput("Status must be between 1 and 5.");
    }
    return static_cast<CollectionStatus>(value);
}

double EWasteItem::conditionMultiplier(Condition condition) {
    switch (condition) {
        case Condition::Working: return 1.25;
        case Condition::PartiallyWorking: return 1.00;
        case Condition::NonWorking: return 0.75;
    }
    throw InvalidInput("Invalid condition for reward calculation.");
}

int EWasteItem::calculateByRate(double typeRate) const {
    return static_cast<int>(std::lround(weight_ * typeRate * conditionMultiplier(condition_)));
}

// ===== Mobile.h =====


class Mobile : public EWasteItem {
public:
    Mobile(const std::string& id,
           const std::string& ownerName,
           const std::string& itemName,
           Condition condition,
           double weight,
           CollectionStatus status = CollectionStatus::Registered,
           const std::string& collectionDate = "",
           const std::string& collectionAddress = "");

    std::string getType() const override;
    std::string getCategory() const override;
    int calculateReward() const override;
    std::string getRecommendedAction() const override;
    double getRewardRate() const override;
};

// ===== Mobile.cpp =====

Mobile::Mobile(const std::string& id,
               const std::string& ownerName,
               const std::string& itemName,
               Condition condition,
               double weight,
               CollectionStatus status,
               const std::string& collectionDate,
               const std::string& collectionAddress)
    : EWasteItem(id, ownerName, itemName, condition, weight, status, collectionDate, collectionAddress) {}

std::string Mobile::getType() const {
    return "Mobile";
}

std::string Mobile::getCategory() const {
    return "Small Electronics";
}

int Mobile::calculateReward() const {
    return calculateByRate(getRewardRate());
}

std::string Mobile::getRecommendedAction() const {
    switch (getCondition()) {
        case Condition::Working:
            return "Reuse / Resale after data wipe";
        case Condition::PartiallyWorking:
            return "Repair / Refurbish and recover usable parts";
        case Condition::NonWorking:
            return "Component Recovery -> Material Recovery -> Certified Recycling";
    }
    return "Assessment required";
}

double Mobile::getRewardRate() const {
    return 18.0;
}

// ===== Laptop.h =====


class Laptop : public EWasteItem {
public:
    Laptop(const std::string& id,
           const std::string& ownerName,
           const std::string& itemName,
           Condition condition,
           double weight,
           CollectionStatus status = CollectionStatus::Registered,
           const std::string& collectionDate = "",
           const std::string& collectionAddress = "");

    std::string getType() const override;
    std::string getCategory() const override;
    int calculateReward() const override;
    std::string getRecommendedAction() const override;
    double getRewardRate() const override;
};

// ===== Laptop.cpp =====

Laptop::Laptop(const std::string& id,
               const std::string& ownerName,
               const std::string& itemName,
               Condition condition,
               double weight,
               CollectionStatus status,
               const std::string& collectionDate,
               const std::string& collectionAddress)
    : EWasteItem(id, ownerName, itemName, condition, weight, status, collectionDate, collectionAddress) {}

std::string Laptop::getType() const {
    return "Laptop";
}

std::string Laptop::getCategory() const {
    return "Computer Equipment";
}

int Laptop::calculateReward() const {
    return calculateByRate(getRewardRate());
}

std::string Laptop::getRecommendedAction() const {
    switch (getCondition()) {
        case Condition::Working:
            return "Reuse / Resale after secure data removal";
        case Condition::PartiallyWorking:
            return "Repair / Refurbish; test storage and memory modules";
        case Condition::NonWorking:
            return "Component Recovery -> Material Recovery -> Recycling";
    }
    return "Assessment required";
}

double Laptop::getRewardRate() const {
    return 25.0;
}

// ===== Battery.h =====


class Battery : public EWasteItem {
public:
    Battery(const std::string& id,
            const std::string& ownerName,
            const std::string& itemName,
            Condition condition,
            double weight,
            CollectionStatus status = CollectionStatus::Registered,
            const std::string& collectionDate = "",
            const std::string& collectionAddress = "");

    std::string getType() const override;
    std::string getCategory() const override;
    int calculateReward() const override;
    std::string getRecommendedAction() const override;
    double getRewardRate() const override;
    bool isHazardous() const override;
    std::string getSafetyWarning() const override;
};

// ===== Battery.cpp =====

Battery::Battery(const std::string& id,
                 const std::string& ownerName,
                 const std::string& itemName,
                 Condition condition,
                 double weight,
                 CollectionStatus status,
                 const std::string& collectionDate,
                 const std::string& collectionAddress)
    : EWasteItem(id, ownerName, itemName, condition, weight, status, collectionDate, collectionAddress) {}

std::string Battery::getType() const {
    return "Battery";
}

std::string Battery::getCategory() const {
    return "Hazardous E-Waste";
}

int Battery::calculateReward() const {
    return calculateByRate(getRewardRate());
}

std::string Battery::getRecommendedAction() const {
    switch (getCondition()) {
        case Condition::Working:
            return "Certified reuse only; hazardous handling is mandatory";
        case Condition::PartiallyWorking:
            return "Controlled assessment/reconditioning; send to hazardous recycler";
        case Condition::NonWorking:
            return "Hazardous Battery Recycling; never place in general waste";
    }
    return "Hazardous assessment required";
}

double Battery::getRewardRate() const {
    return 12.0;
}

bool Battery::isHazardous() const {
    return true;
}

std::string Battery::getSafetyWarning() const {
    return "HAZARDOUS: isolate terminals, avoid heat or puncture, and use an authorised battery recycler.";
}

// ===== Printer.h =====


class Printer : public EWasteItem {
public:
    Printer(const std::string& id,
            const std::string& ownerName,
            const std::string& itemName,
            Condition condition,
            double weight,
            CollectionStatus status = CollectionStatus::Registered,
            const std::string& collectionDate = "",
            const std::string& collectionAddress = "");

    std::string getType() const override;
    std::string getCategory() const override;
    int calculateReward() const override;
    std::string getRecommendedAction() const override;
    double getRewardRate() const override;
};

// ===== Printer.cpp =====

Printer::Printer(const std::string& id,
                 const std::string& ownerName,
                 const std::string& itemName,
                 Condition condition,
                 double weight,
                 CollectionStatus status,
                 const std::string& collectionDate,
                 const std::string& collectionAddress)
    : EWasteItem(id, ownerName, itemName, condition, weight, status, collectionDate, collectionAddress) {}

std::string Printer::getType() const {
    return "Printer";
}

std::string Printer::getCategory() const {
    return "Office Electronics";
}

int Printer::calculateReward() const {
    return calculateByRate(getRewardRate());
}

std::string Printer::getRecommendedAction() const {
    switch (getCondition()) {
        case Condition::Working:
            return "Reuse / Resale after functional testing";
        case Condition::PartiallyWorking:
            return "Repair / Refurbish and recover cartridges/electronics safely";
        case Condition::NonWorking:
            return "Disassembly -> Material Recovery -> Certified Recycling";
    }
    return "Assessment required";
}

double Printer::getRewardRate() const {
    return 15.0;
}

// ===== Television.h =====


class Television : public EWasteItem {
public:
    Television(const std::string& id,
               const std::string& ownerName,
               const std::string& itemName,
               Condition condition,
               double weight,
               CollectionStatus status = CollectionStatus::Registered,
               const std::string& collectionDate = "",
               const std::string& collectionAddress = "");

    std::string getType() const override;
    std::string getCategory() const override;
    int calculateReward() const override;
    std::string getRecommendedAction() const override;
    double getRewardRate() const override;
};

// ===== Television.cpp =====

Television::Television(const std::string& id,
                       const std::string& ownerName,
                       const std::string& itemName,
                       Condition condition,
                       double weight,
                       CollectionStatus status,
                       const std::string& collectionDate,
                       const std::string& collectionAddress)
    : EWasteItem(id, ownerName, itemName, condition, weight, status, collectionDate, collectionAddress) {}

std::string Television::getType() const {
    return "Television";
}

std::string Television::getCategory() const {
    return "Large Electronics";
}

int Television::calculateReward() const {
    return calculateByRate(getRewardRate());
}

std::string Television::getRecommendedAction() const {
    switch (getCondition()) {
        case Condition::Working:
            return "Reuse / Resale after display and safety testing";
        case Condition::PartiallyWorking:
            return "Repair / Refurbish; test display, power and control boards";
        case Condition::NonWorking:
            return "Disassembly -> Component Recovery -> Material Recovery";
    }
    return "Assessment required";
}

double Television::getRewardRate() const {
    return 10.0;
}

// ===== CollectionManager.h =====


struct CollectionInfo {
    std::string date;
    std::string address;
};

class CollectionManager {
private:
    std::map<std::string, CollectionInfo> collectionRecords_;

    static bool isValidDate(const std::string& date);

public:
    void scheduleCollection(EWasteItem& item,
                            const std::string& date,
                            const std::string& address);
    void updateStatus(EWasteItem& item, EWasteItem::CollectionStatus newStatus);
    void updateCollectionInfo(EWasteItem& item,
                              const std::string& date,
                              const std::string& address);
    void registerLoadedItem(const EWasteItem& item);
    bool hasCollectionRecord(const std::string& id) const;
};

// ===== CollectionManager.cpp =====


bool CollectionManager::isValidDate(const std::string& date) {
    static const std::regex datePattern("[0-9]{4}-[0-9]{2}-[0-9]{2}");
    return std::regex_match(date, datePattern);
}

void CollectionManager::scheduleCollection(EWasteItem& item,
                                            const std::string& date,
                                            const std::string& address) {
    if (item.getStatus() != EWasteItem::CollectionStatus::Registered) {
        throw InvalidInput("Only Registered items can receive their first collection schedule.");
    }
    if (!isValidDate(date)) {
        throw InvalidInput("Date must use YYYY-MM-DD format.");
    }
    if (address.empty()) {
        throw InvalidInput("Collection address cannot be empty.");
    }

    item.setCollectionInfo(date, address);
    item.setStatus(EWasteItem::CollectionStatus::Scheduled);
    collectionRecords_[item.getId()] = {date, address};
}

void CollectionManager::updateStatus(EWasteItem& item, EWasteItem::CollectionStatus newStatus) {
    const auto current = item.getStatus();
    if (newStatus == current) {
        return;
    }

    const bool validNextStep =
        (current == EWasteItem::CollectionStatus::Scheduled &&
         newStatus == EWasteItem::CollectionStatus::Collected) ||
        (current == EWasteItem::CollectionStatus::Collected &&
         newStatus == EWasteItem::CollectionStatus::Sorted) ||
        (current == EWasteItem::CollectionStatus::Sorted &&
         newStatus == EWasteItem::CollectionStatus::Recycled);

    if (!validNextStep) {
        throw InvalidInput("Invalid status transition. Required sequence: Scheduled -> Collected -> Sorted -> Recycled.");
    }
    item.setStatus(newStatus);
}

void CollectionManager::updateCollectionInfo(EWasteItem& item,
                                              const std::string& date,
                                              const std::string& address) {
    if (item.getStatus() == EWasteItem::CollectionStatus::Registered) {
        throw InvalidInput("Schedule the Registered item before updating collection information.");
    }
    if (!isValidDate(date)) {
        throw InvalidInput("Date must use YYYY-MM-DD format.");
    }
    if (address.empty()) {
        throw InvalidInput("Collection address cannot be empty.");
    }

    item.setCollectionInfo(date, address);
    collectionRecords_[item.getId()] = {date, address};
}

void CollectionManager::registerLoadedItem(const EWasteItem& item) {
    if (!item.getCollectionDate().empty() && !item.getCollectionAddress().empty()) {
        collectionRecords_[item.getId()] = {item.getCollectionDate(), item.getCollectionAddress()};
    }
}

bool CollectionManager::hasCollectionRecord(const std::string& id) const {
    return collectionRecords_.find(id) != collectionRecords_.end();
}

// ===== RewardManager.h =====


class RewardManager {
public:
    static std::string getRewardLevel(int points);
    void displayReward(const EWasteItem& item) const;
};

// ===== RewardManager.cpp =====


std::string RewardManager::getRewardLevel(int points) {
    if (points >= 500) return "Eco Champion";
    if (points >= 250) return "Gold";
    if (points >= 100) return "Silver";
    return "Bronze";
}

void RewardManager::displayReward(const EWasteItem& item) const {
    const double basePoints = item.getWeight() * item.getRewardRate();
    const double multiplier = EWasteItem::conditionMultiplier(item.getCondition());
    const int totalPoints = item.calculateReward();

    std::cout << "\nReward Calculation\n"
              << "------------------\n"
              << "Item type rate       : " << item.getRewardRate() << " points/kg\n"
              << "Weight               : " << std::fixed << std::setprecision(2)
              << item.getWeight() << " kg\n"
              << "Condition multiplier : " << multiplier << "\n"
              << "Formula              : round(weight x type rate x condition multiplier)\n"
              << "Calculation          : round(" << item.getWeight() << " x "
              << item.getRewardRate() << " x " << multiplier << ")\n"
              << "Reward points        : " << totalPoints << "\n"
              << "Reward level         : " << getRewardLevel(totalPoints) << "\n"
              << std::defaultfloat;

    if (item.isHazardous()) {
        std::cout << "Hazard warning       : " << item.getSafetyWarning() << '\n';
    }
    if (basePoints <= 0.0) {
        std::cout << "Note                 : No positive base value was calculated.\n";
    }
}

// ===== ReportManager.h =====


class ReportManager {
public:
    void generate(const std::vector<EWasteItem*>& items) const;
};

// ===== ReportManager.cpp =====


namespace {
void printCountMap(const std::string& title, const std::map<std::string, int>& values) {
    std::cout << "\n" << title << "\n";
    std::cout << std::string(title.size(), '-') << "\n";
    if (values.empty()) {
        std::cout << "No records\n";
        return;
    }
    for (const auto& entry : values) {
        std::cout << std::left << std::setw(28) << entry.first << " : " << entry.second << '\n';
    }
}
}

void ReportManager::generate(const std::vector<EWasteItem*>& items) const {
    double totalWeight = 0.0;
    int totalReward = 0;
    int hazardousCount = 0;
    int recycledCount = 0;
    std::map<std::string, int> byCategory;
    std::map<std::string, int> byCondition;
    std::map<std::string, int> byStatus;

    for (const EWasteItem* item : items) {
        if (item == nullptr) continue;
        totalWeight += item->getWeight();
        totalReward += item->calculateReward();
        ++byCategory[item->getCategory()];
        ++byCondition[EWasteItem::conditionToString(item->getCondition())];
        ++byStatus[EWasteItem::statusToString(item->getStatus())];
        if (item->isHazardous()) ++hazardousCount;
        if (item->getStatus() == EWasteItem::CollectionStatus::Recycled) ++recycledCount;
    }

    std::cout << "\n========================================\n"
              << "SYSTEM REPORT\n"
              << "========================================\n"
              << "Total registered items : " << items.size() << '\n'
              << "Total weight           : " << std::fixed << std::setprecision(2)
              << totalWeight << " kg\n"
              << "Total reward points    : " << totalReward << '\n'
              << "Overall reward level   : " << RewardManager::getRewardLevel(totalReward) << '\n'
              << "Hazardous items        : " << hazardousCount << '\n'
              << "Recycled items         : " << recycledCount << '\n';

    printCountMap("Items by Category", byCategory);
    printCountMap("Items by Condition", byCondition);
    printCountMap("Items by Status", byStatus);
    std::cout << "========================================\n" << std::defaultfloat;
}

// ===== FileManager.h =====


class FileManager {
public:
    static constexpr const char* DATA_FILE = "ewaste_data.txt";
    static constexpr const char* LOG_FILE = "transactions.log";

    bool loadItems(std::vector<EWasteItem*>& items, std::set<std::string>& ids) const;
    void saveItems(const std::vector<EWasteItem*>& items) const;
    void appendTransaction(const std::string& action,
                           const std::string& itemId,
                           const std::string& details) const;
    void displayTransactionLog() const;

    static EWasteItem* createItem(const std::string& type,
                                  const std::string& id,
                                  const std::string& ownerName,
                                  const std::string& itemName,
                                  EWasteItem::Condition condition,
                                  double weight,
                                  EWasteItem::CollectionStatus status = EWasteItem::CollectionStatus::Registered,
                                  const std::string& collectionDate = "",
                                  const std::string& collectionAddress = "");
    static std::string normalizeType(const std::string& type);
};

// ===== FileManager.cpp =====



namespace {
std::string trim(const std::string& text) {
    const std::string whitespace = " \t\r\n";
    const std::size_t first = text.find_first_not_of(whitespace);
    if (first == std::string::npos) return "";
    const std::size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}

std::string timeStamp() {
    const std::time_t now = std::time(nullptr);
    std::tm localTime{};

    // std::localtime is supported by older MinGW versions as well.
    const std::tm* timeInfo = std::localtime(&now);
    if (timeInfo != nullptr) {
        localTime = *timeInfo;
    }

    std::ostringstream output;
    output << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return output.str();
}
}

std::string FileManager::normalizeType(const std::string& type) {
    std::string result = trim(type);
    for (char& character : result) {
        character = static_cast<char>(std::tolower(static_cast<unsigned char>(character)));
    }
    return result;
}

EWasteItem* FileManager::createItem(const std::string& type,
                                    const std::string& id,
                                    const std::string& ownerName,
                                    const std::string& itemName,
                                    EWasteItem::Condition condition,
                                    double weight,
                                    EWasteItem::CollectionStatus status,
                                    const std::string& collectionDate,
                                    const std::string& collectionAddress) {
    const std::string normalizedType = normalizeType(type);
    if (normalizedType == "mobile") {
        return new Mobile(id, ownerName, itemName, condition, weight, status, collectionDate, collectionAddress);
    }
    if (normalizedType == "laptop") {
        return new Laptop(id, ownerName, itemName, condition, weight, status, collectionDate, collectionAddress);
    }
    if (normalizedType == "battery") {
        return new Battery(id, ownerName, itemName, condition, weight, status, collectionDate, collectionAddress);
    }
    if (normalizedType == "printer") {
        return new Printer(id, ownerName, itemName, condition, weight, status, collectionDate, collectionAddress);
    }
    if (normalizedType == "television" || normalizedType == "tv") {
        return new Television(id, ownerName, itemName, condition, weight, status, collectionDate, collectionAddress);
    }
    throw InvalidEWasteType("Invalid e-waste type. Choose Mobile, Laptop, Battery, Printer, or Television.");
}

bool FileManager::loadItems(std::vector<EWasteItem*>& items, std::set<std::string>& ids) const {
    std::ifstream input(DATA_FILE);
    if (!input) {
        return false;
    }

    std::string id;
    std::string type;
    std::string ownerName;
    std::string itemName;
    std::string collectionDate;
    std::string collectionAddress;
    int conditionValue = 0;
    int statusValue = 0;
    double weight = 0.0;

    while (input >> std::quoted(id)
                 >> std::quoted(type)
                 >> std::quoted(ownerName)
                 >> std::quoted(itemName)
                 >> conditionValue
                 >> weight
                 >> statusValue
                 >> std::quoted(collectionDate)
                 >> std::quoted(collectionAddress)) {
        EWasteItem* item = nullptr;
        try {
            EWasteItem::validateId(id);
            if (ids.find(id) != ids.end()) {
                throw InvalidID("Duplicate ID found while loading: " + id);
            }
            item = createItem(type,
                              id,
                              ownerName,
                              itemName,
                              EWasteItem::conditionFromInt(conditionValue),
                              weight,
                              EWasteItem::statusFromInt(statusValue),
                              collectionDate,
                              collectionAddress);
            items.push_back(item);
            ids.insert(id);
        } catch (const EWasteException& exception) {
            delete item;
            std::cerr << "Skipped invalid saved record: " << exception.what() << '\n';
        }
    }
    return true;
}

void FileManager::saveItems(const std::vector<EWasteItem*>& items) const {
    std::ofstream output(DATA_FILE, std::ios::trunc);
    if (!output) {
        throw EWasteException("Unable to open ewaste_data.txt for saving.");
    }

    output << std::fixed << std::setprecision(2);
    for (const EWasteItem* item : items) {
        if (item == nullptr) continue;
        output << std::quoted(item->getId()) << ' '
               << std::quoted(item->getType()) << ' '
               << std::quoted(item->getOwnerName()) << ' '
               << std::quoted(item->getItemName()) << ' '
               << static_cast<int>(item->getCondition()) << ' '
               << item->getWeight() << ' '
               << static_cast<int>(item->getStatus()) << ' '
               << std::quoted(item->getCollectionDate()) << ' '
               << std::quoted(item->getCollectionAddress()) << '\n';
    }
    if (!output) {
        throw EWasteException("An error occurred while writing ewaste_data.txt.");
    }
}

void FileManager::appendTransaction(const std::string& action,
                                    const std::string& itemId,
                                    const std::string& details) const {
    std::ofstream log(LOG_FILE, std::ios::app);
    if (!log) {
        throw EWasteException("Unable to open transactions.log.");
    }
    log << timeStamp() << " | " << action << " | " << itemId << " | " << details << '\n';
}

void FileManager::displayTransactionLog() const {
    std::ifstream log(LOG_FILE);
    std::cout << "\n========================================\n"
              << "TRANSACTION LOG\n"
              << "========================================\n";
    if (!log) {
        std::cout << "No transaction log exists yet.\n";
        return;
    }

    std::string line;
    while (std::getline(log, line)) {
        std::cout << line << '\n';
    }
}

// ===== main.cpp =====


namespace {

std::string toLower(std::string text) {
    for (char& character : text) {
        character = static_cast<char>(std::tolower(static_cast<unsigned char>(character)));
    }
    return text;
}

std::string readRequiredLine(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    if (!std::getline(std::cin, value)) {
        throw InvalidInput("Input stream closed.");
    }
    if (value.empty()) {
        throw InvalidInput("This field cannot be empty.");
    }
    return value;
}

int readInteger(const std::string& prompt, int minimum, int maximum) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
        throw InvalidInput("Input stream closed.");
    }

    std::stringstream parser(line);
    int value = 0;
    char extra = '\0';
    if (!(parser >> value) || (parser >> extra) || value < minimum || value > maximum) {
        throw InvalidInput("Enter a whole number from " + std::to_string(minimum) +
                           " to " + std::to_string(maximum) + ".");
    }
    return value;
}

double readNumber(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
        throw InvalidInput("Input stream closed.");
    }

    std::stringstream parser(line);
    double value = 0.0;
    char extra = '\0';
    if (!(parser >> value) || (parser >> extra) || !std::isfinite(value)) {
        throw InvalidInput("Enter a finite numeric value.");
    }
    return value;
}

EWasteItem::Condition chooseCondition() {
    std::cout << "\nCondition\n"
              << "1. Working\n"
              << "2. Partially Working\n"
              << "3. Non-Working\n";
    return EWasteItem::conditionFromInt(readInteger("Enter condition: ", 1, 3));
}

std::string chooseType() {
    std::cout << "\nE-Waste Type\n"
              << "1. Mobile\n"
              << "2. Laptop\n"
              << "3. Battery\n"
              << "4. Printer\n"
              << "5. Television\n";
    const int choice = readInteger("Enter type: ", 1, 5);
    switch (choice) {
        case 1: return "Mobile";
        case 2: return "Laptop";
        case 3: return "Battery";
        case 4: return "Printer";
        case 5: return "Television";
    }
    throw InvalidEWasteType("Invalid e-waste type selection.");
}

EWasteItem::CollectionStatus chooseStatus() {
    std::cout << "\nCollection Status\n"
              << "1. Registered\n"
              << "2. Scheduled\n"
              << "3. Collected\n"
              << "4. Sorted\n"
              << "5. Recycled\n";
    return EWasteItem::statusFromInt(readInteger("Enter status: ", 1, 5));
}

std::string generateId(const std::set<std::string>& ids) {
    unsigned long long highest = 1000;
    for (const std::string& id : ids) {
        if (id.rfind("EW", 0) != 0) continue;
        try {
            const unsigned long long number = std::stoull(id.substr(2));
            if (number > highest) highest = number;
        } catch (const std::exception&) {
            // IDs are validated before being placed in the set.
        }
    }
    return "EW" + std::to_string(highest + 1);
}

EWasteItem* findById(std::vector<EWasteItem*>& items, const std::string& id) {
    EWasteItem::validateId(id);
    for (EWasteItem* item : items) {
        if (item != nullptr && item->getId() == id) return item;
    }
    throw ItemNotFound("No e-waste item was found with ID " + id + ".");
}

void printCompact(const EWasteItem* item) {
    std::cout << std::left << std::setw(9) << item->getId()
              << std::setw(13) << item->getType()
              << std::setw(22) << item->getItemName().substr(0, 20)
              << std::setw(18) << EWasteItem::conditionToString(item->getCondition())
              << std::setw(13) << EWasteItem::statusToString(item->getStatus())
              << std::setw(9) << item->calculateReward() << '\n';
}

void displayDetails(const EWasteItem& item) {
    std::cout << "\n----------------------------------------\n"
              << "E-WASTE ITEM DETAILS\n"
              << "----------------------------------------\n"
              << "ID                 : " << item.getId() << '\n'
              << "Owner              : " << item.getOwnerName() << '\n'
              << "Item name          : " << item.getItemName() << '\n'
              << "Type               : " << item.getType() << '\n'
              << "Automatic category : " << item.getCategory() << '\n'
              << "Condition          : " << EWasteItem::conditionToString(item.getCondition()) << '\n'
              << "Weight             : " << std::fixed << std::setprecision(2)
              << item.getWeight() << " kg\n"
              << "Collection status  : " << EWasteItem::statusToString(item.getStatus()) << '\n'
              << "Reward points      : " << item.calculateReward() << '\n'
              << "Reward level       : " << RewardManager::getRewardLevel(item.calculateReward()) << '\n'
              << "Recommendation     : " << item.getRecommendedAction() << '\n'
              << "Collection date    : "
              << (item.getCollectionDate().empty() ? "Not scheduled" : item.getCollectionDate()) << '\n'
              << "Collection address : "
              << (item.getCollectionAddress().empty() ? "Not scheduled" : item.getCollectionAddress()) << '\n'
              << "Safety note        : " << item.getSafetyWarning() << '\n'
              << "----------------------------------------\n"
              << std::defaultfloat;
}

void viewAll(const std::vector<EWasteItem*>& items) {
    if (items.empty()) {
        std::cout << "\nNo e-waste records are registered.\n";
        return;
    }

    std::vector<EWasteItem*> rankedItems = items;
    std::sort(rankedItems.begin(), rankedItems.end(),
              [](const EWasteItem* left, const EWasteItem* right) {
                  return *left > *right;
              });

    std::cout << "\n========================================\n"
              << "ALL E-WASTE RECORDS (highest reward first)\n"
              << "========================================\n"
              << std::left << std::setw(9) << "ID"
              << std::setw(13) << "Type"
              << std::setw(22) << "Item"
              << std::setw(18) << "Condition"
              << std::setw(13) << "Status"
              << std::setw(9) << "Points" << '\n'
              << std::string(84, '-') << '\n';
    for (const EWasteItem* item : rankedItems) {
        printCompact(item);
    }
}

void searchItems(std::vector<EWasteItem*>& items) {
    const int searchChoice = readInteger(
        "\nSearch by: 1. E-Waste ID  2. Owner name  3. Category\nEnter choice: ", 1, 3);

    if (searchChoice == 1) {
        const std::string id = readRequiredLine("Enter E-Waste ID: ");
        displayDetails(*findById(items, id));
        return;
    }

    const std::string query = toLower(readRequiredLine(
        searchChoice == 2 ? "Enter owner name or part of it: " : "Enter category: "));
    bool found = false;
    for (EWasteItem* item : items) {
        const std::string searchable = searchChoice == 2
            ? toLower(item->getOwnerName())
            : toLower(item->getCategory());
        if ((searchChoice == 2 && searchable.find(query) != std::string::npos) ||
            (searchChoice == 3 && searchable == query)) {
            displayDetails(*item);
            found = true;
        }
    }
    if (!found) {
        throw ItemNotFound("No matching e-waste item was found.");
    }
}

void registerEWaste(std::vector<EWasteItem*>& items,
                    std::set<std::string>& ids,
                    const FileManager& fileManager) {
    const std::string owner = readRequiredLine("Owner name: ");
    const std::string type = chooseType();
    const std::string itemName = readRequiredLine("Item name/model: ");
    const EWasteItem::Condition condition = chooseCondition();
    const double weight = readNumber("Weight in kg: ");
    if (weight <= 0.0 || weight > 1000.0) {
        throw InvalidWeight("Weight must be greater than 0 kg and no more than 1000 kg.");
    }

    const std::string id = generateId(ids);
    std::unique_ptr<EWasteItem> newItem(
        FileManager::createItem(type, id, owner, itemName, condition, weight));
    EWasteItem* itemForLog = newItem.get();
    items.push_back(newItem.release());
    ids.insert(id);

    fileManager.appendTransaction("REGISTER", id,
                                  itemForLog->getType() + " / " + itemForLog->getItemName());
    std::cout << "\nRegistered successfully. Generated ID: " << id << '\n'
              << "Automatic classification: " << itemForLog->getCategory() << '\n'
              << "Initial recommendation: " << itemForLog->getRecommendedAction() << '\n';
    if (itemForLog->isHazardous()) {
        std::cout << "WARNING: " << itemForLog->getSafetyWarning() << '\n';
    }
}

void assessCondition(EWasteItem& item, const FileManager& fileManager) {
    const EWasteItem::Condition newCondition = chooseCondition();
    item.setCondition(newCondition);
    fileManager.appendTransaction("ASSESS", item.getId(),
                                  EWasteItem::conditionToString(newCondition));
    std::cout << "\nCondition updated to " << EWasteItem::conditionToString(newCondition) << ".\n"
              << "Recommendation: " << item.getRecommendedAction() << '\n';
    if (item.isHazardous()) {
        std::cout << "WARNING: " << item.getSafetyWarning() << '\n';
    }
}

void showRecommendation(const EWasteItem& item) {
    std::cout << "\nRecommendation for " << item.getId() << " (" << item.getType() << ")\n"
              << "Condition: " << EWasteItem::conditionToString(item.getCondition()) << '\n'
              << "Action   : " << item.getRecommendedAction() << '\n';
    if (item.isHazardous()) {
        std::cout << "WARNING  : " << item.getSafetyWarning() << '\n';
    }
}

void scheduleCollection(EWasteItem& item,
                        CollectionManager& collectionManager,
                        const FileManager& fileManager) {
    const std::string date = readRequiredLine("Collection date (YYYY-MM-DD): ");
    const std::string address = readRequiredLine("Collection address: ");
    collectionManager.scheduleCollection(item, date, address);
    fileManager.appendTransaction("SCHEDULE", item.getId(), date + " / " + address);
    std::cout << "Collection scheduled. Status is now Scheduled.\n";
}

void updateStatus(EWasteItem& item,
                  CollectionManager& collectionManager,
                  const FileManager& fileManager) {
    const EWasteItem::CollectionStatus newStatus = chooseStatus();
    collectionManager.updateStatus(item, newStatus);
    fileManager.appendTransaction("STATUS", item.getId(),
                                  EWasteItem::statusToString(newStatus));
    std::cout << "Status updated to " << EWasteItem::statusToString(newStatus) << ".\n";
}

void updateCollectionInfo(EWasteItem& item,
                          CollectionManager& collectionManager,
                          const FileManager& fileManager) {
    const std::string date = readRequiredLine("New collection date (YYYY-MM-DD): ");
    const std::string address = readRequiredLine("New collection address: ");
    collectionManager.updateCollectionInfo(item, date, address);
    fileManager.appendTransaction("COLLECTION_UPDATE", item.getId(), date + " / " + address);
    std::cout << "Collection information updated.\n";
}

void updateItem(std::vector<EWasteItem*>& items,
                CollectionManager& collectionManager,
                const FileManager& fileManager) {
    const std::string id = readRequiredLine("Enter E-Waste ID to update: ");
    EWasteItem* item = findById(items, id);
    const int choice = readInteger(
        "1. Condition\n2. Status\n3. Collection information\n4. All applicable fields\nEnter choice: ",
        1, 4);

    if (choice == 1 || choice == 4) {
        assessCondition(*item, fileManager);
    }
    if (choice == 2 || choice == 4) {
        updateStatus(*item, collectionManager, fileManager);
    }
    if (choice == 3 || (choice == 4 && item->getStatus() != EWasteItem::CollectionStatus::Registered)) {
        updateCollectionInfo(*item, collectionManager, fileManager);
    }
}

void deleteItem(std::vector<EWasteItem*>& items,
                std::set<std::string>& ids,
                const FileManager& fileManager) {
    const std::string id = readRequiredLine("Enter E-Waste ID to delete: ");
    EWasteItem* item = findById(items, id);
    const std::string confirmation = readRequiredLine(
        "Delete " + item->getItemName() + " permanently? (Y/N): ");
    if (confirmation.empty() ||
        (confirmation[0] != 'Y' && confirmation[0] != 'y')) {
        std::cout << "Delete cancelled.\n";
        return;
    }

    const std::string deletedName = item->getItemName();
    const auto iterator = std::find(items.begin(), items.end(), item);
    delete *iterator;
    items.erase(iterator);
    ids.erase(id);
    fileManager.appendTransaction("DELETE", id, deletedName);
    std::cout << "Item deleted successfully.\n";
}

void printProjectHeader() {
    const std::string border(96, '#');

    std::cout << border << '\n'
              << R"(#                                                                                              #
#       EEEEE   -   W   W   AAAAA   SSSSS   TTTTT   EEEEE                                      #
#       E       -   W   W   A   A   S         T     E                                          #
#       EEEE    -   W W W   AAAAA   SSSSS     T     EEEE                                       #
#       E       -   W W W   A   A       S     T     E                                          #
#       EEEEE   -   W   W   A   A   SSSSS     T     EEEEE                                      #
#                                                                                              #
#             E-WASTE MANAGEMENT & RECYCLING SYSTEM                                            #
#       An Object-Oriented Approach to Sustainable E-Waste Management                          #
#                                                                                              #
#  Student : Kaushik B. Topale                                                                 #
#  Course  : Diploma Electronics & Computer Engineering                                        #
#  Subject : Object-Oriented Programming in C++                                                #)"
              << '\n' << border << "\n\n";
}

void printMenu() {
    std::cout << "\n========================================\n"
              << "E-WASTE MANAGEMENT SYSTEM\n"
              << "========================================\n"
              << "1.  Register E-Waste\n"
              << "2.  View All E-Waste\n"
              << "3.  Search E-Waste\n"
              << "4.  Assess Condition\n"
              << "5.  Get Recycling Recommendation\n"
              << "6.  Calculate Reward\n"
              << "7.  Schedule Collection\n"
              << "8.  Update Status\n"
              << "9.  Generate Reports\n"
              << "10. Update E-Waste\n"
              << "11. Delete E-Waste\n"
              << "12. View Transaction Log\n"
              << "13. Save & Exit\n"
              << "----------------------------------------\n";
}

} // namespace

int main() {
    std::vector<EWasteItem*> items; // Base-class pointers demonstrate runtime polymorphism.
    std::set<std::string> ids;
    FileManager fileManager;
    CollectionManager collectionManager;
    RewardManager rewardManager;
    ReportManager reportManager;

    printProjectHeader();

    try {
        const bool dataFileFound = fileManager.loadItems(items, ids);
        for (const EWasteItem* item : items) {
            collectionManager.registerLoadedItem(*item);
        }
        if (dataFileFound) {
            std::cout << "Loaded " << items.size() << " saved item(s) from "
                      << FileManager::DATA_FILE << ".\n";
        } else {
            std::cout << "No saved data found. Starting a new e-waste register.\n";
        }
    } catch (const EWasteException& exception) {
        std::cerr << "Startup warning: " << exception.what() << '\n';
    }

    bool running = true;
    bool saved = false;
    while (running && std::cin) {
        printMenu();
        try {
            const int choice = readInteger("Enter choice: ", 1, 13);
            switch (choice) {
                case 1:
                    registerEWaste(items, ids, fileManager);
                    break;
                case 2:
                    viewAll(items);
                    break;
                case 3:
                    searchItems(items);
                    break;
                case 4: {
                    EWasteItem* item = findById(items, readRequiredLine("Enter E-Waste ID: "));
                    assessCondition(*item, fileManager);
                    break;
                }
                case 5: {
                    EWasteItem* item = findById(items, readRequiredLine("Enter E-Waste ID: "));
                    showRecommendation(*item);
                    break;
                }
                case 6: {
                    EWasteItem* item = findById(items, readRequiredLine("Enter E-Waste ID: "));
                    rewardManager.displayReward(*item);
                    break;
                }
                case 7: {
                    EWasteItem* item = findById(items, readRequiredLine("Enter E-Waste ID: "));
                    scheduleCollection(*item, collectionManager, fileManager);
                    break;
                }
                case 8: {
                    EWasteItem* item = findById(items, readRequiredLine("Enter E-Waste ID: "));
                    updateStatus(*item, collectionManager, fileManager);
                    break;
                }
                case 9:
                    reportManager.generate(items);
                    break;
                case 10:
                    updateItem(items, collectionManager, fileManager);
                    break;
                case 11:
                    deleteItem(items, ids, fileManager);
                    break;
                case 12:
                    fileManager.displayTransactionLog();
                    break;
                case 13:
                    fileManager.saveItems(items);
                    fileManager.appendTransaction("SAVE", "SYSTEM",
                                                  std::to_string(items.size()) + " item(s) saved");
                    std::cout << "Data saved to " << FileManager::DATA_FILE << ". Goodbye.\n";
                    saved = true;
                    running = false;
                    break;
            }
        } catch (const EWasteException& exception) {
            std::cout << "\nOperation cancelled: " << exception.what() << '\n';
        } catch (const std::exception& exception) {
            std::cout << "\nUnexpected standard-library error: " << exception.what() << '\n';
        }
    }

    if (!saved) {
        try {
            fileManager.saveItems(items);
            std::cout << "Data saved automatically before closing.\n";
        } catch (const std::exception& exception) {
            std::cerr << "Final save failed: " << exception.what() << '\n';
        }
    }

    for (EWasteItem* item : items) {
        delete item;
    }
    items.clear();
    return 0;
}
