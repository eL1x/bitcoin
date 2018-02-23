#include <random>
#include <bitcoin/bitcoin.hpp>

// prefix to search
const std::string searchPrefix = "1sky";

// functions prototypes
bc::ec_secret generateRandomSecret(std::default_random_engine& engine);
std::string generateBitcoinAddress(const bc::ec_secret& secret);
bool checkIfMatchFound(const std::string& address);

int main()
{
	std::random_device random;
	std::default_random_engine engine(random());

	// Loop to mining vanity addresses
	while (true)
	{
		bc::ec_secret secret = generateRandomSecret(engine);
		std::string address = generateBitcoinAddress(secret);

		if (checkIfMatchFound(address))
		{
			// Match found
			std::cout << "Vanity address was found! " << address << std::endl;
			std::cout << "Secret(hex): " << bc::encode_base16(secret) << std::endl;
		}
	}

	return 0;
}

// Generate random secret key - random 32 bytes
bc::ec_secret generateRandomSecret(std::default_random_engine& engine)
{
	bc::ec_secret secret;

	// Setting random value to every byte
	for (uint8_t& byte : secret)
		byte = engine() % std::numeric_limits<uint8_t>::max();

	return secret;
}

// Generate bitcoin address from EC secret
std::string generateBitcoinAddress(const bc::ec_secret& secret)
{
	bc::wallet::ec_private privateKey(secret);
	bc::wallet::payment_address payAddr(privateKey);

	return payAddr.encoded();
}

// Check if address match search pattern
bool checkIfMatchFound(const std::string& address)
{
	auto addrIt = address.begin();

	for (auto it = searchPrefix.begin(); it != searchPrefix.end(); ++it, ++addrIt)
		if (*it != std::tolower(*addrIt))
			return false; // Address doesn't match

	return true; // Address match
}