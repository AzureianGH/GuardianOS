#include <libhydrix/hfile/ide.h>
#include <libhydrix/hfile/ata.h>
#include <libhydrix/libhydrix.h>
Console* ErrrConsole;
unsigned char ide_buf[2048] = {0};
volatile unsigned static char ide_irq_invoked = 0;
unsigned static char atapi_packet[12] = {0xA8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

IDEChannelRegisters channels[2];
void SetIDEConsole(Console* console) {
    ErrrConsole = console;
}
void ide_write(unsigned char channel, unsigned char reg, unsigned char data) {
   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
   if (reg < 0x08)
      PortIO::OutByte(channels[channel].base  + reg - 0x00, data);
   else if (reg < 0x0C)
      PortIO::OutByte(channels[channel].base  + reg - 0x06, data);
   else if (reg < 0x0E)
      PortIO::OutByte(channels[channel].ctrl  + reg - 0x0A, data);
   else if (reg < 0x16)
      PortIO::OutByte(channels[channel].bmide + reg - 0x0E, data);
   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
}

unsigned char ide_read(unsigned char channel, unsigned char reg) {
   unsigned char result;
   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);
   if (reg < 0x08)
      result = PortIO::InByte(channels[channel].base + reg - 0x00);
   else if (reg < 0x0C)
      result = PortIO::InByte(channels[channel].base  + reg - 0x06);
   else if (reg < 0x0E)
      result = PortIO::InByte(channels[channel].ctrl  + reg - 0x0A);
   else if (reg < 0x16)
      result = PortIO::InByte(channels[channel].bmide + reg - 0x0E);
   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
   return result;
}

unsigned char ide_polling(unsigned char channel, unsigned int advanced_check) {

   // (I) Delay 400 nanosecond for BSY to be set:
   // -------------------------------------------------
   for(int i = 0; i < 4; i++)
      ide_read(channel, ATA_REG_ALTSTATUS); // Reading the Alternate Status port wastes 100ns; loop four times.

   // (II) Wait for BSY to be cleared:
   // -------------------------------------------------
   while (ide_read(channel, ATA_REG_STATUS) & ATA_SR_BSY)
      ; // Wait for BSY to be zero.

   if (advanced_check) {
      unsigned char state = ide_read(channel, ATA_REG_STATUS); // Read Status Register.

      // (III) Check For Errors:
      // -------------------------------------------------
      if (state & ATA_SR_ERR)
         return 2; // Error.

      // (IV) Check If Device fault:
      // -------------------------------------------------
      if (state & ATA_SR_DF)
         return 1; // Device Fault.

      // (V) Check DRQ:
      // -------------------------------------------------
      // BSY = 0; DF = 0; ERR = 0 so we should check for DRQ now.
      if ((state & ATA_SR_DRQ) == 0)
         return 3; // DRQ should be set

   }

   return 0; // No Error.

}
struct ide_device {
   unsigned char  Reserved;    // 0 (Empty) or 1 (This Drive really exists).
   unsigned char  Channel;     // 0 (Primary Channel) or 1 (Secondary Channel).
   unsigned char  Drive;       // 0 (Master Drive) or 1 (Slave Drive).
   unsigned short Type;        // 0: ATA, 1:ATAPI.
   unsigned short Signature;   // Drive Signature
   unsigned short Capabilities;// Features.
   unsigned int   CommandSets; // Command Sets Supported.
   unsigned int   Size;        // Size in Sectors.
   unsigned char  Model[41];   // Model in string.
} ide_devices[4];

unsigned char ide_print_error(unsigned int drive, unsigned char err) {
   if (err == 0)
      return err;

   ErrrConsole->WriteLine("IDE:");
   if (err == 1) {ErrrConsole->WriteLine("- Device Fault\n     "); err = 19;}
   else if (err == 2) {
      unsigned char st = ide_read(ide_devices[drive].Channel, ATA_REG_ERROR);
      if (st & ATA_ER_AMNF)   {ErrrConsole->WriteLine("- No Address Mark Found\n     ");   err = 7;}
      if (st & ATA_ER_TK0NF)   {ErrrConsole->WriteLine("- No Media or Media Error\n     ");   err = 3;}
      if (st & ATA_ER_ABRT)   {ErrrConsole->WriteLine("- Command Aborted\n     ");      err = 20;}
      if (st & ATA_ER_MCR)   {ErrrConsole->WriteLine("- No Media or Media Error\n     ");   err = 3;}
      if (st & ATA_ER_IDNF)   {ErrrConsole->WriteLine("- ID mark not Found\n     ");      err = 21;}
      if (st & ATA_ER_MC)   {ErrrConsole->WriteLine("- No Media or Media Error\n     ");   err = 3;}
      if (st & ATA_ER_UNC)   {ErrrConsole->WriteLine("- Uncorrectable Data Error\n     ");   err = 22;}
      if (st & ATA_ER_BBK)   {ErrrConsole->WriteLine("- Bad Sectors\n     ");       err = 13;}
   } else  if (err == 3)           {ErrrConsole->WriteLine("- Reads Nothing\n     "); err = 23;}
     else  if (err == 4)  {ErrrConsole->WriteLine("- Write Protected\n     "); err = 8;}
   ErrrConsole->WriteLine(((StringObj)"- [%s %s] %s\n" + (const char *[]){" [Primary] ", " [Secondary] "}[ide_devices[drive].Channel] + (const char *[]){" {Master} ", " {Slave} "}[ide_devices[drive].Drive] + " " + ide_devices[drive].Model).c_str());

   return err;
}

void ide_read_buffer(unsigned char channel, unsigned char reg, unsigned char* buffer,
                     unsigned int quads) {
   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, ATA_REG_CONTROL, 0x80 | channels[channel].nIEN);

   // Save ES register and set it to DS
   unsigned short es;
   asm volatile("movw %%es, %0" : "=r"(es));
   asm volatile("movw %%ds, %%ax; movw %%ax, %%es" ::: "ax");

   if (reg < 0x08)
      PortIO::InSL(channels[channel].base  + reg - 0x00, (void*)buffer, quads);
   else if (reg < 0x0C)
      PortIO::InSL(channels[channel].base  + reg - 0x06, (void*)buffer, quads);
   else if (reg < 0x0E)
      PortIO::InSL(channels[channel].ctrl  + reg - 0x0A, (void*)buffer, quads);
   else if (reg < 0x16)
      PortIO::InSL(channels[channel].bmide + reg - 0x0E, (void*)buffer, quads);

   // Restore ES register
   asm volatile("movw %0, %%es" :: "r"(es));

   if (reg > 0x07 && reg < 0x0C)
      ide_write(channel, ATA_REG_CONTROL, channels[channel].nIEN);
}

void IDEInit(unsigned int BAR0, unsigned int BAR1, unsigned int BAR2, unsigned int BAR3, unsigned int BAR4) {

   int j, k, count = 0;

   // 1- Detect I/O Ports which interface IDE Controller:
   channels[ATA_PRIMARY  ].base  = (BAR0 & 0xFFFFFFFC) + 0x1F0 * (!BAR0);
   channels[ATA_PRIMARY  ].ctrl  = (BAR1 & 0xFFFFFFFC) + 0x3F6 * (!BAR1);
   channels[ATA_SECONDARY].base  = (BAR2 & 0xFFFFFFFC) + 0x170 * (!BAR2);
   channels[ATA_SECONDARY].ctrl  = (BAR3 & 0xFFFFFFFC) + 0x376 * (!BAR3);
   channels[ATA_PRIMARY  ].bmide = (BAR4 & 0xFFFFFFFC) + 0; // Bus Master IDE
   channels[ATA_SECONDARY].bmide = (BAR4 & 0xFFFFFFFC) + 8; // Bus Master IDE
   // 2- Disable IRQs:
   ide_write(ATA_PRIMARY  , ATA_REG_CONTROL, 2);
   ide_write(ATA_SECONDARY, ATA_REG_CONTROL, 2);
 // 3- Detect ATA-ATAPI Devices:
   ErrrConsole->Clear();
   ErrrConsole->WriteLine("Detecting IDE Devices");
   ErrrConsole->graphics->DisplayNonSynced();

   for (int i = 0; i < 2; i++)
      for (j = 0; j < 2; j++) {

         unsigned char err = 0, type = IDE_ATA, status;
         ide_devices[count].Reserved = 0; // Assuming that no drive here.

         // (I) Select Drive:
         ide_write(i, ATA_REG_HDDEVSEL, 0xA0 | (j << 4)); // Select Drive.
         int timer = TimeSinceBootMS();
         while (TimeSinceBootMS() < timer); // Wait 1ms for drive select to work.

         // (II) Send ATA Identify Command:
         ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
         //USe TimeSinceBootMS(); to wait exactly 1 ms by comparing the time

         int timer2 = TimeSinceBootMS();
         while (TimeSinceBootMS() < timer2); // This method avoids the use of PIT Timer since IRQs got disabled.

         ErrrConsole->WriteLine("Waiting for 1 ms");
         ErrrConsole->graphics->DisplayNonSynced();
         
          // This function should be implemented in your OS. which waits for 1 ms.
                   // it is based on System Timer Device Driver.

         // (III) Polling:
         if (ide_read(i, ATA_REG_STATUS) == 0) continue; // If Status = 0, No Device.

         while(1) {
            status = ide_read(i, ATA_REG_STATUS);
            if ((status & ATA_SR_ERR)) {err = 1; break;} // If Err, Device is not ATA.
            if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ)) break; // Everything is right.
         }

         // (IV) Probe for ATAPI Devices:

         if (err != 0) {
            unsigned char cl = ide_read(i, ATA_REG_LBA1);
            unsigned char ch = ide_read(i, ATA_REG_LBA2);

            if (cl == 0x14 && ch == 0xEB)
               type = IDE_ATAPI;
            else if (cl == 0x69 && ch == 0x96)
               type = IDE_ATAPI;
            else
               continue; // Unknown Type (may not be a device).

            ide_write(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
            int timer3 = TimeSinceBootMS();
            while (TimeSinceBootMS() < timer3);
         }

         // (V) Read Identification Space of the Device:
         ide_read_buffer(i, ATA_REG_DATA, ide_buf, 128);

         // (VI) Read Device Parameters:
         ide_devices[count].Reserved     = 1;
         ide_devices[count].Type         = type;
         ide_devices[count].Channel      = i;
         ide_devices[count].Drive        = j;
         ide_devices[count].Signature    = *((unsigned short *)(ide_buf + ATA_IDENT_DEVICETYPE));
         ide_devices[count].Capabilities = *((unsigned short *)(ide_buf + ATA_IDENT_CAPABILITIES));
         ide_devices[count].CommandSets  = *((unsigned int *)(ide_buf + ATA_IDENT_COMMANDSETS));

         // (VII) Get Size:
         if (ide_devices[count].CommandSets & (1 << 26))
            // Device uses 48-Bit Addressing:
            ide_devices[count].Size   = *((unsigned int *)(ide_buf + ATA_IDENT_MAX_LBA_EXT));
         else
            // Device uses CHS or 28-bit Addressing:
            ide_devices[count].Size   = *((unsigned int *)(ide_buf + ATA_IDENT_MAX_LBA));

         // (VIII) String indicates model of device (like Western Digital HDD and SONY DVD-RW...):
         for(k = 0; k < 40; k += 2) {
            ide_devices[count].Model[k] = ide_buf[ATA_IDENT_MODEL + k + 1];
            ide_devices[count].Model[k + 1] = ide_buf[ATA_IDENT_MODEL + k];}
         ide_devices[count].Model[40] = 0; // Terminate String.

         //print name of the device
         
         ErrrConsole->WriteLine(((StringObj)"Found " + ide_devices[count].Model).c_str());
         ErrrConsole->graphics->DisplayNonSynced();

         count++;
      }

   // 4- Print Summary:
   for (int i = 0; i < 4; i++)
      if (ide_devices[i].Reserved == 1) {
         ErrrConsole->WriteLineS(((StringObj)" Found " + (const char *[]){"ATA", "ATAPI"}[ide_devices[i].Type] + " GB: " + (ide_devices[i].Size / 1024 / 1024 / 2) + " Model: "  + ide_devices[i].Model).c_str());
      }
}