#include <bitcoin/bitcoin.hpp>

int main()
{
	// Decode private key from hex to raw format
	bc::ec_secret rawFormat;
	bc::decode_base16(rawFormat, "E9873D79C6D87DC0FB6A5778633389F4453213303DA61F20BD67FC233AA33262");

	bc::wallet::ec_private secret(rawFormat, bc::wallet::ec_private::mainnet_p2kh);

	// Generate public key from private key and showing it in compressed format
	bc::wallet::ec_public publicKey(secret);
	std::cout << "Public key: " << publicKey.encoded() << std::endl;

	// Get public key compressed to publicKeyData, compute has of public key for P2PKH address
	bc::data_chunk publicKeyData;
	publicKey.to_data(publicKeyData);
	const auto hash = bc::bitcoin_short_hash(publicKeyData); // RIPEMD160(SHA256(publicKey))

	// Encode public key to Base58Check encoding
	bc::data_chunk unencodedAddress;
	unencodedAddress.reserve(25);
	unencodedAddress.push_back(0); // 0 - BTC address version byte
	bc::extend_data(unencodedAddress, hash); // add public key hash
	bc::append_checksum(unencodedAddress); // Compute version+hash checksum
	assert(unencodedAddress.size() == 25);
	const std::string address = bc::encode_base58(unencodedAddress); // Base58 encoding

	std::cout << "BTC address: " << address << std::endl;
	return 0;
}