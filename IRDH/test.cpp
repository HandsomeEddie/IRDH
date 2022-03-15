#include "test.h"
#include <set>

template <size_t k, size_t nTaps>
std::vector<bool> LFSR(const std::bitset<k> start, const std::array<uint16_t, nTaps> taps, const bool wrapping = false, const bool verbose = false) {
    std::vector<bool> result;
    //Sanity-checks
    if (taps.size() < 2)
    {
        std::cerr << "At least two taps are needed" << std::endl;
        return result;
    }
    for (auto tap : taps)
    {
        if (tap > k)
        {
            std::cerr << "Out of range tap " << tap << std::endl;
            return result;
        }
    }
    if (start.none())
    {
        std::cerr << "A non-zero start value is needed" << std::endl;
        return result;
    }

    const uint32_t maxPeriod = pow(2, k) - 1;
    result.reserve(maxPeriod);

    std::set<uint32_t> lfsrHistory;
    std::bitset<k> lfsr(start);
    uint32_t i = 0;
    do
    {
        // XOR of all the tapped bits. We use -1 because an exponent N in the monic polynomial corresponds to bitset index N-1
        bool newBit = lfsr[taps.at(0) - 1];
        for (uint16_t j = 1; j < taps.size(); ++j)
        {
            newBit ^= lfsr[taps.at(j) - 1];
        }

        lfsr <<= 1;
        lfsr[0] = newBit;
        if (verbose)
        {
            std::cout << i << " " << newBit << " " << lfsr << std::endl;
        }
        result.emplace_back(newBit);
        ++i;

        if (!wrapping)
        {
            if (lfsrHistory.count(lfsr.to_ulong()))
            {
                break;
            }
            lfsrHistory.insert(lfsr.to_ulong());
        }
    } while (lfsr != start && i < maxPeriod);
    result.shrink_to_fit();//only some special taps, namely the PRBS-k, will lead to the maxPeriod, others will stop earlier
    return result;
}


int main() {
    // ---------------------------- Content owner ----------------------------
    FileReader fileReader;
    std::string filePath = Utils::GetInstance()->GetPath();
    std::string imageName = filePath + "/../image/Airplane.bmp";
    //std::string imageName = "C:/Users/fudis/Documents/GitHub/IRDH/image/Airplane.bmp";
    std::string data = "Hello World";

    fileReader.ReadImage(imageName);

    Image originImage = fileReader.GetImage();

    // ---------------------------- Data hider ----------------------------
    DataHider dataHider;
    dataHider.Init(originImage, data);
    dataHider.Embed();
    Image maskImage = dataHider.GetImage();
    HidingKey hidingKey = dataHider.GetKey();

    fileReader.WriteImage(maskImage, filePath + "/../image/MaskImage.bmp");

    // ---------------------------- Receiver ----------------------------
    Receiver receiver;
    receiver.Init(maskImage, hidingKey);
    receiver.Extract();
    std::string extractData = receiver.GetData();
    Image recoverImage = receiver.GetImage();

    fileReader.WriteImage(recoverImage, filePath + "/../image/RecoverImage.bmp");

    // ------------------------------ test ----------------------------------

    std::cout << "The embedded data is: " << data << std::endl;
    std::cout << "The extracted data is: " << extractData << std::endl;

    SeedGenerator seedGenerator;
    //seedGenerator.RandomBinary();


    //PRBS3
    std::array<uint16_t, 2> taps3 = { 2, 3 }; // Exponents of the monic polynomial
    auto prbs3 = LFSR(std::bitset<3>().flip(), taps3);// Start value all high

    //PRBS4
    std::array<uint16_t, 2> taps4 = { 3, 4 }; // Exponents of the monic polynomial
    auto prbs4 = LFSR(std::bitset<4>().flip(), taps4);// Start value all high

    //PRBS7
    std::array<uint16_t, 2> taps7 = { 7, 6 }; // Exponents of the monic polynomial
    auto prbs7 = LFSR(std::bitset<7>().flip(), taps7);// Start value all high

    //PRBS11
    std::array<uint16_t, 2> taps11 = { 11, 9 }; // Exponents of the monic polynomial
    auto prbs11 = LFSR(std::bitset<11>().flip(), taps11);// Start value all high

    //PRBS15
    std::array<uint16_t, 2> taps15 = { 15, 14 }; // Exponents of the monic polynomial
    auto prbs15 = LFSR(std::bitset<15>().flip(), taps15);// Start value all high

    //PRBS31
    std::array<uint16_t, 2> taps31 = { 31, 28 };
  // auto prbs31 = LFSR(std::bitset<31>().flip(), taps31);

    for (auto prbs : { prbs3, prbs4, prbs7, prbs15 })
    {
        std::cout << prbs.size() << ":\t";
        for (auto p : prbs)
        {
            std::cout << p << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Over~";
}