#pragma once

int myHelperPort = 0;
SOCKET mServer = INVALID_SOCKET;
sockaddr_in sadr_in;

class Tools
{
public:
	SOCKET createSocket(int port)
	{
		SOCKET mSocket = INVALID_SOCKET;
		WSAData WSADat;
		WSAStartup(0x0202, &WSADat);
		sadr_in.sin_family = AF_INET;
		sadr_in.sin_addr.s_addr = 0;
		sadr_in.sin_port = htons(port);
		mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (mSocket != INVALID_SOCKET)
			bind(mSocket, (sockaddr*)&sadr_in, sizeof(sadr_in));

		return mSocket;
	}

	string GetData(const char* recv_data, char* data, char to)
	{
		if (!strstr(recv_data, data))
			return string("0");

		string ret = "";
		for (int i = string(recv_data).find(data) + strlen(data); i <= string(recv_data).length(); i++)
		{
			if ((recv_data[i] == to) || (i == string(recv_data).length()))
			{
				return ret;
			}
			ret = string(ret + recv_data[i]);
		}
		return string("0");
	}

	wchar_t *To_Unicode(const char*utf8_string)
	{
		int err;
		wchar_t * res;
		int res_len = MultiByteToWideChar(CP_UTF8, 0, utf8_string, -1, NULL, 0);
		if (res_len == 0) { return NULL; }
		res = (wchar_t *)calloc(sizeof(wchar_t), res_len);
		if (res == NULL) { return NULL; }
		err = MultiByteToWideChar(CP_UTF8, 0, utf8_string, -1, res, res_len);
		if (err == 0)
		{
			free(res);
			return NULL;
		}
		return res;
	}

	char *To_CP1251(wchar_t *unicode_string)
	{
		int err;
		char * res;
		int res_len = WideCharToMultiByte(1251, 0, unicode_string, -1, NULL, 0, NULL, NULL);
		if (res_len == 0) return NULL;
		res = (char *)calloc(sizeof(char), res_len);
		if (res == NULL) return NULL;
		err = WideCharToMultiByte(1251, 0, unicode_string, -1, res, res_len, NULL, NULL);
		if (err == 0)
		{
			free(res);
			return NULL;
		}
		return res;
	}

	string GetData2C(const char* recv_data, char *from, char *to)
	{
		if (!strstr(recv_data, from))
			return "";

		if (!strstr(recv_data, to))
			return "";

		string str = string(recv_data);

		if (str.find_first_of(from) < 0 || str.find_first_of(from) == -1)
			return "";

		string str1 = string(str.begin() + str.find_first_of(from) + 1, str.end());

		if (str1.find_first_of(to) < 0 || str1.find_first_of(to) == -1)
			return "";

		return string(str1.begin(), str1.begin() + str1.find_first_of(to));
	}

	static string base64_encode(const std::string &in)
	{
		std::string out;

		int val = 0, valb = -6;
		for (char c : in) {
			val = (val << 8) + c;
			valb += 8;
			while (valb >= 0) {
				out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
				valb -= 6;
			}
		}
		if (valb>-6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
		while (out.size() % 4) out.push_back('=');
		return out;
	}
}*sTools;