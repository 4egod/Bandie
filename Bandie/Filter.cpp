
#include "stdafx.h"
#include "Filter.h"

HANDLE Filter::handle;
HANDLE Filter::thread;

void Filter::Start()
{
	Filter::thread = CreateThread(NULL, 1, (LPTHREAD_START_ROUTINE)Filter::start, (LPVOID)handle, 0, NULL);
	if (thread == NULL)
	{
		fprintf(stderr, "Error: failed to start thread (%u)\n", GetLastError());
		exit(EXIT_FAILURE);
	}
}

void Filter::Stop()
{
	WinDivertClose(handle);
	CloseHandle(thread);
}

void Filter::start()
{
	unsigned char packet[MAXBUF];
	WINDIVERT_ADDRESS addr;
	UINT packet_len;
	PWINDIVERT_IPHDR ip_header;
	PWINDIVERT_TCPHDR tcp_header;

	handle = WinDivertOpen("inbound and tcp.Syn", WINDIVERT_LAYER_NETWORK, 0, 0);
	if (handle == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "error: failed to open the WinDivert device (%d)\n", GetLastError());
		exit(EXIT_FAILURE);
	}

	while (handle)
	{
		if (!WinDivertRecv(handle, packet, sizeof(packet), &addr, &packet_len))
		{
			fprintf(stderr, "warning: failed to read packet (%d)\n", GetLastError());
			continue;
		}

		if (WinDivertHelperParsePacket(packet, packet_len, &ip_header,
			NULL, NULL, NULL, &tcp_header, NULL, NULL, NULL))
		{
			UINT8 *src_addr = (UINT8 *)&ip_header->SrcAddr;
			UINT8 *dst_addr = (UINT8 *)&ip_header->DstAddr;

			if (addr.Direction == WINDIVERT_DIRECTION_INBOUND && tcp_header->Ack == 1 && tcp_header->Syn == 1)
			{
				//printf("IPv4 [Version=%u HdrLength=%u TOS=%u Length=%u Id=0x%.4X "
				//	"Reserved=%u DF=%u MF=%u FragOff=%u TTL=%u Protocol=%u "
				//	"Checksum=0x%.4X SrcAddr=%u.%u.%u.%u DstAddr=%u.%u.%u.%u]\n",
				//	ip_header->Version, ip_header->HdrLength,
				//	ntohs(ip_header->TOS), ntohs(ip_header->Length),
				//	ntohs(ip_header->Id), WINDIVERT_IPHDR_GET_RESERVED(ip_header),
				//	WINDIVERT_IPHDR_GET_DF(ip_header),
				//	WINDIVERT_IPHDR_GET_MF(ip_header),
				//	ntohs(WINDIVERT_IPHDR_GET_FRAGOFF(ip_header)), ip_header->TTL,
				//	ip_header->Protocol, ntohs(ip_header->Checksum),
				//	src_addr[0], src_addr[1], src_addr[2], src_addr[3],
				//	dst_addr[0], dst_addr[1], dst_addr[2], dst_addr[3]);

				//printf("TCP [SrcPort=%u DstPort=%u SeqNum=%u AckNum=%u "
				//	"HdrLength=%u Reserved1=%u Reserved2=%u Urg=%u Ack=%u "
				//	"Psh=%u Rst=%u Syn=%u Fin=%u Window=%u Checksum=0x%.4X "
				//	"UrgPtr=%u]\n",
				//	ntohs(tcp_header->SrcPort), ntohs(tcp_header->DstPort),
				//	ntohl(tcp_header->SeqNum), ntohl(tcp_header->AckNum),
				//	tcp_header->HdrLength, tcp_header->Reserved1,
				//	tcp_header->Reserved2, tcp_header->Urg, tcp_header->Ack,
				//	tcp_header->Psh, tcp_header->Rst, tcp_header->Syn,
				//	tcp_header->Fin, ntohs(tcp_header->Window),
				//	ntohs(tcp_header->Checksum), ntohs(tcp_header->UrgPtr));

				tcp_header->Window = htons(5);

				WinDivertHelperCalcChecksums(packet, packet_len, &addr, NULL);
			}
		}

		if (!WinDivertSend(handle, packet, packet_len, &addr, NULL))
		{
			fprintf(stderr, "warning: failed to reinject packet (%d)\n", GetLastError());
		}
	}
}
