#pragma once

SOCKET dedicHelper = 0;
bool IsSocketAvai = false;

class Tools
{
public:
	static string base64_encode(const std::string &in)
	{
		std::string out;

		int val = 0, valb = -6;
		for (char c : in) {
			val = (val << 8) + c;
			valb += 8;
			while (valb >= 0) {
				out.push_back(XorStr("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/")[(val >> valb) & 0x3F]);
				valb -= 6;
			}
		}
		if (valb>-6) out.push_back(XorStr("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/")[((val << 8) >> (valb + 8)) & 0x3F]);
		while (out.size() % 4) out.push_back('=');
		return out;
	}

	SOCKET ConnectToDedicHelper(const char* helperIP, int helperPort)
	{
		SOCKET dhelperS = INVALID_SOCKET;

		WSAData WSADat;
		WSAStartup(0x0202, &WSADat);
		sockaddr_in sock_in;
		sock_in.sin_family = AF_INET;
		sock_in.sin_addr.s_addr = inet_addr(helperIP);
		sock_in.sin_port = htons(helperPort);
		dhelperS = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(dhelperS, (sockaddr*)&sock_in, sizeof(sock_in)) == SOCKET_ERROR)
		{
			closesocket(dhelperS);
			//WSACleanup();
			IsSocketAvai = false;
		}
		else
		{
			IsSocketAvai = true;
			return dhelperS;
		}
	}

	char *stream_read(int fd)
	{
		uint8_t *msg = NULL;
		uint8_t *curr_pos = NULL;

		size_t buff_size = 256;
		msg = (uint8_t *)calloc(buff_size, 1);
		curr_pos = msg;

		do {
			size_t size = recv(fd, (char*)curr_pos, buff_size / 2, 0);

			if ((size <= 0) || (size == SOCKET_ERROR))
			{
				free(msg);
				return NULL;
			}

			curr_pos += size;

			if (curr_pos[-1] != '}' && curr_pos[-1] != '\0')
			{
				uint8_t *old_msg = msg;
				buff_size = buff_size * 2;
				msg = (uint8_t *)realloc(msg, buff_size);
				curr_pos = (curr_pos - old_msg) + msg;
				memset(curr_pos, 0, buff_size - (curr_pos - msg));
			}

		} while (curr_pos[-1] != '}' && curr_pos[-1] != '\0');

		return (char *)msg;
	}
}*sTools;

DWORD WINAPI CRecvFromDH()
{
	int zero_num = 0;
	while (1)
	{
		if (IsSocketAvai)
		{
			fd_set readfds;
			FD_ZERO(&readfds);

			FD_SET(dedicHelper, &readfds);
			timeval tv = { 0, 0 };
			select(0, &readfds, 0, 0, &tv);

			if (FD_ISSET(dedicHelper, &readfds))
			{
				char* recv_data = sTools->stream_read(dedicHelper);
				if (recv_data == NULL)
				{
					zero_num++;
					if (zero_num > 14)
					{
						closesocket(dedicHelper);
						//WSACleanup();
						zero_num = 0;
						IsSocketAvai = false;
					}
				}
			}
			FD_CLR(dedicHelper, &readfds);
		}
		Sleep(100);
	}
	return 0;
}