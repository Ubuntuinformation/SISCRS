use std::io::{self, Write};
use std::process::exit;
use std::path::PathBuf;
use std::env;
use std::fs;

fn main() {
    let mut current_dir = PathBuf::from("/");
    
    println!("╔════════════════════════════════════════════════╗");
    println!("║         SiscrShell v2.0 - Fortini OS           ║");
    println!("║     Type 'help' for a list of commands         ║");
    println!("╚════════════════════════════════════════════════╝");
    println!();
    
    let _stdin = io::stdin();
    
    loop {
        print!("{}> ", current_dir.display());
        io::stdout().flush().unwrap();
        
        let mut input = String::new();
        if io::stdin().read_line(&mut input).is_err() {
            break;
        }
        
        let input = input.trim();
        if input.is_empty() {
            continue;
        }
        
        let parts: Vec<&str> = input.split_whitespace().collect();
        if parts.is_empty() {
            continue;
        }
        
        let cmd = parts[0];
        let args = &parts[1..];
        
        match cmd {
            // Navigation & filesystem
            "pwd" => println!("{}", current_dir.display()),
            
            "cd" => {
                if args.is_empty() {
                    println!("cd: missing directory argument");
                } else {
                    let path = PathBuf::from(args[0]);
                    let new_dir = if path.is_absolute() {
                        path
                    } else {
                        current_dir.join(&path)
                    };
                    
                    if new_dir.exists() && new_dir.is_dir() {
                        current_dir = new_dir;
                    } else {
                        println!("cd: no such directory: {}", args[0]);
                    }
                }
            }
            
            "ls" => {
                let path = if args.is_empty() {
                    current_dir.clone()
                } else {
                    let p = PathBuf::from(args[0]);
                    if p.is_absolute() { p } else { current_dir.join(&p) }
                };
                
                if path.exists() {
                    if path.is_dir() {
                        if let Ok(entries) = fs::read_dir(&path) {
                            for entry in entries {
                                if let Ok(entry) = entry {
                                    let name = entry.file_name();
                                    let is_dir = entry.file_type().map(|ft| ft.is_dir()).unwrap_or(false);
                                    if is_dir {
                                        println!("[DIR] {}/", name.to_string_lossy());
                                    } else {
                                        println!("[FILE] {}", name.to_string_lossy());
                                    }
                                }
                            }
                        }
                    } else {
                        println!("[FILE] {}", path.display());
                    }
                } else {
                    println!("ls: cannot access '{}': No such file or directory", args.join(" "));
                }
            }
            
            "mkdir" => {
                if args.is_empty() {
                    println!("mkdir: missing directory argument");
                } else {
                    for dir_name in args {
                        let path = current_dir.join(dir_name);
                        if let Err(e) = fs::create_dir(&path) {
                            println!("mkdir: cannot create directory '{}': {}", dir_name, e);
                        }
                    }
                }
            }
            
            "rm" => {
                if args.is_empty() {
                    println!("rm: missing file argument");
                } else {
                    for file_name in args {
                        let path = current_dir.join(file_name);
                        if path.exists() {
                            if let Err(e) = fs::remove_file(&path) {
                                println!("rm: cannot remove '{}': {}", file_name, e);
                            }
                        } else {
                            println!("rm: cannot remove '{}': No such file", file_name);
                        }
                    }
                }
            }
            
            "touch" => {
                if args.is_empty() {
                    println!("touch: missing file argument");
                } else {
                    for file_name in args {
                        let path = current_dir.join(file_name);
                        if let Err(e) = fs::File::create(&path) {
                            println!("touch: cannot create '{}': {}", file_name, e);
                        }
                    }
                }
            }
            
            "cat" => {
                if args.is_empty() {
                    println!("cat: missing file argument");
                } else {
                    for file_name in args {
                        let path = current_dir.join(file_name);
                        match fs::read_to_string(&path) {
                            Ok(contents) => print!("{}", contents),
                            Err(e) => println!("cat: cannot open '{}': {}", file_name, e),
                        }
                    }
                }
            }
            
            // System info
            "echo" => {
                println!("{}", args.join(" "));
            }
            
            "whoami" => {
                if let Ok(user) = env::var("USER") {
                    println!("{}", user);
                } else {
                    println!("root");
                }
            }
            
            "uname" => {
                if args.contains(&"-a") {
                    println!("Fortini OS 2.0 (x86 32-bit) kernel_version=1.0");
                } else {
                    println!("Fortini");
                }
            }
            
            "date" => {
                println!("[System time not available in kernel]");
            }
            
            "uptime" => {
                println!("[System uptime: <kernel dependent>]");
            }
            
            // File operations
            "cp" => {
                if args.len() < 2 {
                    println!("cp: missing source or destination file");
                } else {
                    let src = current_dir.join(args[0]);
                    let dst = current_dir.join(args[1]);
                    if let Err(e) = fs::copy(&src, &dst) {
                        println!("cp: cannot copy '{}' to '{}': {}", args[0], args[1], e);
                    }
                }
            }
            
            "mv" => {
                if args.len() < 2 {
                    println!("mv: missing source or destination file");
                } else {
                    let src = current_dir.join(args[0]);
                    let dst = current_dir.join(args[1]);
                    if let Err(e) = fs::rename(&src, &dst) {
                        println!("mv: cannot move '{}' to '{}': {}", args[0], args[1], e);
                    }
                }
            }
            
            "chmod" => {
                if args.len() < 2 {
                    println!("chmod: missing mode or file");
                } else {
                    println!("[chmod not fully implemented in user mode]");
                }
            }
            
            "chown" => {
                if args.len() < 2 {
                    println!("chown: missing owner or file");
                } else {
                    println!("[chown not fully implemented in user mode]");
                }
            }
            
            // Process-like commands
            "ps" => {
                println!("PID    COMMAND");
                println!("---    -------");
                println!("1      siscrshell");
                println!("[kernel processes hidden]");
            }
            
            "kill" => {
                if args.is_empty() {
                    println!("kill: missing PID argument");
                } else {
                    println!("[kill command not fully available in kernel environment]");
                }
            }
            
            // Network commands (placeholders)
            "ifconfig" => {
                println!("eth0: <RUNNING>");
                println!("  IP: 192.168.1.100");
                println!("  Netmask: 255.255.255.0");
                println!("[Network stack not available in kernel]");
            }
            
            "ping" => {
                if args.is_empty() {
                    println!("ping: missing hostname/IP");
                } else {
                    println!("[Network stack not available - simulate ping]");
                    for i in 1..=4 {
                        println!("Ping {}: {} bytes from {}", i, 32, args[0]);
                    }
                }
            }
            
            // Remote commands (placeholders)
            "ssh" => {
                println!("[SSH not available in kernel environment]");
            }
            
            "telnet" => {
                println!("[Telnet not available in kernel environment]");
            }
            
            // Development tools (placeholders)
            "gcc" => {
                println!("[GCC cross-compiler placeholder]");
                println!("gcc version 11.4.0 (for Fortini OS x86)");
            }
            
            "clang" => {
                println!("[Clang cross-compiler placeholder]");
                println!("clang version 14.0.0");
            }
            
            "make" => {
                println!("[GNU Make placeholder]");
                println!("make: No targets specified and no makefile found");
            }
            
            "gdb" => {
                println!("[GDB debugger placeholder]");
                println!("GDB (Fortini kernel debugger) 11.1");
            }
            
            "valgrind" => {
                println!("[Valgrind memory profiler - not available]");
            }
            
            "strace" => {
                if args.is_empty() {
                    println!("strace: missing command");
                } else {
                    println!("[System call tracer - kernel environment]");
                }
            }
            
            // Info commands
            "help" => {
                print_help();
            }
            
            "version" => {
                println!("SiscrShell v2.0");
                println!("Fortini OS Kernel v1.0");
                println!("Build: December 2024");
            }
            
            "clear" => {
                print!("\x1B[2J\x1B[1;1H");
                io::stdout().flush().unwrap();
            }
            
            "export" => {
                if args.is_empty() {
                    println!("[Environment variables - kernel limited]");
                } else {
                    println!("[export not fully implemented]");
                }
            }
            
            "set" => {
                println!("[set command not fully implemented]");
            }
            
            // Exit
            "exit" | "quit" => {
                println!("Goodbye!");
                exit(0);
            }
            
            // Unknown command
            _ => {
                println!("siscrshell: command not found: {}", cmd);
                println!("Type 'help' for a list of available commands");
            }
        }
    }
}

fn print_help() {
    println!("\n╔════════════════════════════════════════════════╗");
    println!("║            SiscrShell v2.0 - Help              ║");
    println!("╚════════════════════════════════════════════════╝\n");
    
    println!("NAVIGATION & FILESYSTEM:");
    println!("  pwd              - Print working directory");
    println!("  cd <dir>         - Change directory");
    println!("  ls [dir]         - List files and directories");
    println!("  mkdir <dir>      - Create directory");
    println!("  rm <file>        - Remove file");
    println!("  touch <file>     - Create empty file");
    println!("  cat <file>       - Display file contents");
    println!("  cp <src> <dst>   - Copy file");
    println!("  mv <src> <dst>   - Move/rename file");
    println!("  chmod <mode> <f> - Change file permissions");
    println!("  chown <own> <f>  - Change file owner\n");
    
    println!("SYSTEM INFORMATION:");
    println!("  echo <text>      - Print text");
    println!("  whoami           - Show current user");
    println!("  uname [-a]       - Show system information");
    println!("  date             - Show system date/time");
    println!("  uptime           - Show system uptime");
    println!("  ps               - List running processes");
    println!("  kill <pid>       - Terminate process\n");
    
    println!("NETWORK COMMANDS:");
    println!("  ifconfig         - Show network configuration");
    println!("  ping <host>      - Ping host");
    println!("  ssh <host>       - SSH remote connection");
    println!("  telnet <host>    - Telnet connection\n");
    
    println!("DEVELOPMENT TOOLS:");
    println!("  gcc              - GCC compiler");
    println!("  clang            - Clang compiler");
    println!("  make             - GNU Make build tool");
    println!("  gdb              - GDB debugger");
    println!("  valgrind         - Memory profiler");
    println!("  strace           - System call tracer\n");
    
    println!("OTHER COMMANDS:");
    println!("  help             - Show this help message");
    println!("  version          - Show version information");
    println!("  clear            - Clear screen");
    println!("  export <var>     - Set environment variable");
    println!("  set              - Set shell options");
    println!("  exit/quit        - Exit shell\n");
}
