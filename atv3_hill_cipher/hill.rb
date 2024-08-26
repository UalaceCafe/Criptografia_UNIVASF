require 'optparse'
require_relative 'src/hill'

ARGV << '-h' if ARGV.empty?

options = {}
OptionParser.new do |parser|
  parser.banner = "Usage: hill.rb --[enc|dec]=input --out=output --key=key"

  parser.on("--enc=filename", "Encrypt the input file") do |input|
    raise ArgumentError, "Input file not provided" if input.nil?

    options[:encrypt] = true
    options[:input] = input
  end

  parser.on("--dec=filename", "Decrypt the input file") do |input|
    raise ArgumentError, "Input file not provided" if input.nil?

    options[:decrypt] = true
    options[:input] = input
  end

  parser.on("--out=filename", "Output file (optional)") do |output|
    options[:output] = output
  end

  parser.on("--key=filename", "Key file") do |key|
    options[:key] = key
  end

  parser.on("-h", "--help", "Prints this help") do
    puts parser
    exit
  end
end.parse!

if options[:output].nil?
  options[:output] = options[:input].sub(/\.\w+$/, "_out.txt")
end

if (options[:encrypt] && options[:decrypt]) || (!options[:encrypt] && !options[:decrypt])
  raise ArgumentError, "you must provide either `--enc` or `--dec`"
  exit
elsif options[:encrypt]
  puts "> Reading key from file `#{options[:key]}`"
  key = Hill.key_from_file(options[:key])

  puts "> Encrypting file `#{options[:input]}`"
  plain_text = File.read(options[:input])
  cipher_text = Hill.encrypt(plain_text, key)

  puts "> Writing cipher text to file `#{options[:output]}`"
  File.write(options[:output], cipher_text)
elsif options[:decrypt]
  puts "> Reading key from file `#{options[:key]}`"
  key = Hill.key_from_file(options[:key])

  puts "> Decrypting cipher text `#{options[:input]}`"
  cipher_text = File.read(options[:input])
  plain_text = Hill.decrypt(cipher_text, key)

  puts "> Writing plaint text to file `#{options[:output]}`"
  File.write(options[:output], plain_text)
end

puts "All done! :)"
